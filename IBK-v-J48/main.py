###############################################################################
#                      COMP 135 PROGRAMMING ASSIGNMENT 1                      #
###############################################################################

# Data Used - EEG Eye State Data Set in following format:
#           - EEGTestingData_##.arff
#           - EEGTrainingData_##.arff
import matplotlib
matplotlib.use('Agg')
import shutil
import glob, os # https://stackoverflow.com/a/3964691
import traceback
from random import randint
from copy import deepcopy
import os
import re
from weka.classifiers import Classifier, Evaluation
import weka.core.jvm as jvm
from weka.core.converters import Loader#TextDirectoryLoader
import matplotlib.pyplot as plt
import pandas as pd

# For each TRAIN/TEST file combo (denoted by ## for matching features)
class P1:

    # INIT for class (runs project) - !arr initializes empty data field
    def __init__(self,load):

        self.cls = None # Reuseable
        self.evl = None
        self.data = []
        self.IBK = {}
        self.J48 = {}
        if load:
            self.loader = Loader(classname="weka.core.converters.ArffLoader")
            self.parseARFF("data")

    def run(self):

        self.run1(1)
        self.run2()

        # for i in self.J48:
        #     print "J48 - "+str(i)
        #     for j in self.J48[i]:
        #         print str(j)

        self.graph()

    ###########################################################################
    #                           GRAPHING METHODS                              #
    ########################################################################### 

    def graph(self):

        if os.path.exists(os.getcwd()+'/graphs'):
            shutil.rmtree(os.getcwd()+'/graphs')
        # Removed extant
        os.makedirs(os.getcwd()+'/graphs')

        plt1 = pd.DataFrame({'features': map(str,range(14,95,10)),
                                'J48acc':self.J48[1],
                                'IBKacc':self.IBK[1]})
        plt2 = pd.DataFrame({'examples': map(int,range(50,501,50)),
                             'xaxis': map(float,range(0,10)),
                                'J48std14':self.J48["14AVG"]['stdev'], 
                                'J48acc14':self.J48["14AVG"]['acc'],
                                'IBKstd14':self.IBK["14AVG"]['stdev'], 
                                'IBKacc14':self.IBK["14AVG"]['acc'],
                                'J48std54':self.J48["54AVG"]['stdev'], 
                                'J48acc54':self.J48["54AVG"]['acc'],
                                'IBKstd54':self.IBK["54AVG"]['stdev'], 
                                'IBKacc54':self.IBK["54AVG"]['acc']})

        self.graph1(plt1)
        self.graph2(plt2)
    
    def graph1(self,plt1):
        # 1st graph: IBK accuracy over # of features
        ax = plt1[['features', 'J48acc']].plot(x='features', linestyle='-', marker='+')
        plt1[['features', 'J48acc']].plot(x='features', kind='bar', ax=ax)
        plt.ylim(0.7,0.8)
        plt.title("J48 accuracy over features")
        plt.ylabel("Accuracy")
        plt.xlabel("# Features")
        plt.show()
        plt.savefig('graphs/J48.png')
        plt.gcf().clear()
        # 2nd graph: J48 accuracy over # of features
        ax = plt1[['features', 'IBKacc']].plot(x='features', linestyle='-', marker='+')
        plt1[['features', 'IBKacc']].plot(x='features', kind='bar', ax=ax)
        plt.ylim(0.4,0.8)
        plt.title("IBK accuracy over features")
        plt.ylabel("Accuracy")
        plt.xlabel("# Features")
        plt.show()
        plt.savefig('graphs/IBK.png')
        plt.gcf().clear()

    def graph2(self,plt2):
        # Setting the positions and width for the bars
        pos = list(range(len(plt2['IBKacc14'])))
        width = 0.2

        # Plotting the bars
        fig, ax = plt.subplots(figsize=(10,5))

        # Create a bar with pre_score data,
        # in position pos,
        plt.bar(pos,plt2['IBKacc14'], width, alpha=0.5, color='#EE3224', yerr=plt2['IBKstd14'], label="IBKacc14")
        plt.bar([p + width for p in pos],plt2['IBKacc54'],width, alpha=0.5, color='#F78F1E', yerr=plt2['IBKstd54'], label="IBKacc54")
        plt.bar([p + width*2 for p in pos],plt2['J48acc14'],width, alpha=0.5, color='#FFC222', yerr=plt2['J48std14'], label="J48acc14")
        plt.bar([p + width*3 for p in pos],plt2['J48acc54'],width, alpha=0.5, color='green', yerr=plt2['J48std54'], label="J48acc54")

        # Set the position of the x ticks
        ax.set_xticks([p + 1.5 * width for p in pos])

        # Set the labels for the x ticks
        ax.set_xticklabels(plt2['examples'])

        # Setting the x-axis and y-axis limits
        plt.xlim(min(pos)-width, max(pos)+width*4)
        plt.ylim(0.0,1.3)

        plt.title("J48 v IBK Performance Over Training Sample Size")
        plt.ylabel("Accuracy")
        plt.xlabel("# Examples")
        # Shrink current axis by 20%
        box = ax.get_position()
        ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
        # Put a legend to the right of the current axis
        ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
        plt.show()
        plt.savefig('graphs/14_54.png')
        plt.gcf().clear()

    ###########################################################################
    #                    DATA-GENERATING METHODS                              #
    ########################################################################### 

    def run1(self,num):

        self.gen1(num) # Has imported saved in init object w /data/ presumption
        self.data1 = deepcopy(self.data)

    def run2(self):

        self.IBK["14AVG"] = {'acc':[0.0]*10,'stdev':[0.0]*10}
        self.IBK["54AVG"] = {'acc':[0.0]*10,'stdev':[0.0]*10}
        self.J48["14AVG"] = {'acc':[0.0]*10,'stdev':[0.0]*10}
        self.J48["54AVG"] = {'acc':[0.0]*10,'stdev':[0.0]*10}

        # Loops through algorithm 10 times and generates avg, stdev along folds
        for i in range(0,10):
            self.data = self.data1
            self.gen2()

            for i,v in enumerate(self.IBK[14]):
                self.IBK["14AVG"]['acc'][i] += v['acc']
                self.IBK["14AVG"]['stdev'][i] += v['stdev']
            for i,v in enumerate(self.IBK[54]):
                self.IBK["54AVG"]['acc'][i] += v['acc']
                self.IBK["54AVG"]['stdev'][i] += v['stdev']
            for i,v in enumerate(self.J48[14]):
                self.J48["14AVG"]['acc'][i] += v['acc']
                self.J48["14AVG"]['stdev'][i] += v['stdev']
            for i,v in enumerate(self.J48[54]):
                self.J48["54AVG"]['acc'][i] += v['acc']
                self.J48["54AVG"]['stdev'][i] += v['stdev']

        self.IBK["14AVG"]['acc'] = [(i/10.0) for i in self.IBK["14AVG"]['acc']]
        self.IBK["14AVG"]['stdev'] = [(i/10.0) for i in self.IBK["14AVG"]['stdev']]
        self.J48["14AVG"]['acc'] = [(i/10.0) for i in self.J48["14AVG"]['acc']]
        self.J48["14AVG"]['stdev'] = [(i/10.0) for i in self.J48["14AVG"]['stdev']]
        self.IBK["54AVG"]['acc'] = [(i/10.0) for i in self.IBK["54AVG"]['acc']]
        self.IBK["54AVG"]['stdev'] = [(i/10.0) for i in self.IBK["54AVG"]['stdev']]
        self.J48["54AVG"]['acc'] = [(i/10.0) for i in self.J48["54AVG"]['acc']]
        self.J48["54AVG"]['stdev'] = [(i/10.0) for i in self.J48["54AVG"]['stdev']]

    def gen1(self,label):

        self.IBK[label] = []
        self.J48[label]= []

        for x in range(0,9):

            # Train IBk
            self.train(True,self.data[x+9]['data'])
            stash1 = self.test1(self.data[x]['data'])
            self.IBK[label].append(stash1)

            # Train J48
            self.train(False,self.data[x+9]['data'])
            stash2 = self.test1(self.data[x]['data'])
            self.J48[label].append(stash2)
    
    def gen2(self):

        # Make temp folder to hold train- and test- .arffs
        if os.path.exists(os.getcwd()+'/temp'):
            shutil.rmtree(os.getcwd()+'/temp')
        # Removed extant
        os.makedirs(os.getcwd()+'/temp')

        # For the 14 feature and 54-feature sets ALONE:
        test = []
        for i in [0,4]:

            test.append(self.data[i]['file']) # Add file to test set
            train = self.data[i+9]
            raw = self.getARFFpieces(train['file'])

            temp = {}
            temp['header'] = raw['header']
            for k in range(50,501,50):
                temp['train'] = self.getXrandom(k,raw['train'])
                self.genARFF(i,k,temp)
        
        self.parseARFF("temp") # Changes self.data permanently

        freshtests = self.parseSomeARFF(test)

        storeall = self.data

        self.data = [i for i in self.data if "14" in i['file']]
        self.data.append(freshtests[0]) # Adds the 14-feature TEST set
        self.gen3(14)

        self.data = [i for i in storeall if "54" in i['file']]
        self.data.append(freshtests[1]) # Adds the 54-feature TEST set
        self.gen3(54)

    def gen3(self,label):

        self.IBK[label] = []
        self.J48[label]= []

        for x in range(0,10):

            # Train IBk
            self.train(True,self.data[x]['data'])
            stash1 = self.test2(self.data[10]['data'])
            self.IBK[label].append(stash1)

            # Train J48
            self.train(False,self.data[x]['data'])
            stash2 = self.test2(self.data[10]['data'])
            self.J48[label].append(stash2)    

    ###########################################################################
    #                    TRAINING & TESTING METHODS                           #
    ########################################################################### 

    # Stashes newly trained classifier into self.cls attribute
    def train(self,IBk,xtrain):

        if IBk:
            self.cls = Classifier(classname="weka.classifiers.lazy.IBk") # TODO - options?
        else:
            self.cls = Classifier(classname="weka.classifiers.trees.J48", options=["-B"])

        self.cls.build_classifier(xtrain) #Builds with train set for next step

        self.evl = Evaluation(xtrain)

    def test1(self,xtest):
        if self.cls == None:
            print ("Fail: no classifier to test with\n")
        self.evl.test_model(self.cls,xtest)
        return self.findacc(self.evl.confusion_matrix)

    def test2(self,xtest):
        if self.cls == None:
            print ("Fail: no classifier to test with\n")
        self.evl.test_model(self.cls,xtest)
        return {'acc': self.findacc(self.evl.confusion_matrix), 
                'stdev': self.evl.root_mean_squared_error}

    def findacc(self,cmatrix):
        # a+d / a+b+c+d
        x = map(float,filter(None,[re.sub("[^0-9]","",i) for i in str(cmatrix).split('.')]))
        return (x[0]+x[3])/(sum(x))

    ###########################################################################
    #                    ARFF PARSING & HANDLING METHODS                      #
    ###########################################################################

    # Pulls header and particular data examples apart (rms empty lines)
    # & returns as a dict of arrays
    def getARFFpieces(self,fname1):# TODO - clean components

        temp = {'header':[],'train':[]}

        for file in glob.glob('data/*.arff'):
            if (file == fname1): #TRAIN
                with open(os.getcwd()+'/'+file) as f:
                    rest = False
                    for line in f:
                        if line[0] == "@":
                            temp['header'].append(line)
                            if "@DATA" in line:
                                rest = True
                        elif rest and len(line) > 3: # Cutoff for EOF lines
                            temp['train'].append(line)
        return temp

    # Create new ARFF files from which to import
    def genARFF(self,i,k,data):

        if i == 0:
            path = "14-"
        else:
            path = "54-"
        if k == 50:
            path += "0"
        path += str(k)+".arff"

        with open((os.getcwd()+"/temp/Train"+path), "w") as train:
            for j in data['header']:
                train.write(j)
            for j in data['train']:
                train.write(j)
        
    # Get a random x # of documents from a list & return
    def getXrandom(self,x,arr):
        ret = []
        used = []
        max = len(arr) - 1 # Max randint value
        if (max+1) < x:
            print "Err: can't take "+str(x)+" values from a set of "+str(max)
            return
        while len(ret) < x:
            new = randint(0,max) # Shuffle functional
            if new not in used: # make sure to not repeat
                used.append(new)
                ret.append(arr[new]) # Saves that line
        return ret

    # Stashes loaded Instances to data array given parameter for subdirectory
    def parseARFF(self,param):

        self.data = [] # CLEARS FROM PREVIOUS CALL - needed in special iteration case
        try:
            for file in glob.glob(str(param)+"/*.arff"):
                #with open(os.getcwd()+"/"+file, 'r') as f:
                temp = self.loader.load_file(file)
                temp.class_is_last()
                self.data.append({'file':file,'data':temp})
        except:
            print "Missing data/ folder in cwd\n"

    def parseSomeARFF(self,paths):

        ret = []
        for path in paths:
            temp = self.loader.load_file(os.getcwd()+'/'+path)
            temp.class_is_last()
            ret.append({'file':path, 'data':temp})
        return ret

if __name__ == "__main__":
    try:
        jvm.start()
        trialrun = P1(True)
        trialrun.run()
    except Exception, e:
        print(traceback.format_exc())
    finally:
        jvm.stop()
'''
self.chart1 = {'title': "IBk Algorithm Sensitivity",
               'xaxis': "Number of Features",
               'yaxis': "Accuracy",
               'plots': [] }
            self.chart2 = {'title': "J48 Algorithm Sensitivity",
               'xaxis': "Number of Features",
               'yaxis': "Accuracy",
               'plots': [] }
            self.chart3 = {'title': "IBk & J48 Learning Curve Performance",
                'xaxis': "Training Set Size",
                'yaxis': "Accuracy",
                'y2axis': "Standard Deviation",
                'plots': {'IBk-a': [],
                          'IBk-sd': [],
                          'J48-a': [],
                          'J48-sd': []}} # Need to all be plotted on same chart
'''