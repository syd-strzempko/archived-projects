###############################################################################
#                      COMP 135 PROGRAMMING ASSIGNMENT 4                      #
###############################################################################

# Data Used - Optdigits information in following :
#           - 838.arff
#           - optdigits_train.arff
#           - optdigits_test.arff
 
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import shutil
import glob, os, re # https://stackoverflow.com/a/3964691
import traceback
from copy import deepcopy
import pandas as pd
from numpy import random
from math import e

LRATE = 0.1

def main():
    train1 = getfile("838.arff")
    train2 = getfile("optdigits_train.arff")
    test2 = getfile("optdigits_test.arff")
    #  1 838 set
    first = Learner()
    terr = first.learn1(train1)
    # At end, print representation of 1-hot output units
    print first.nodes[1]
    graph1(terr)
    # 2 - other two optdigits (train,test)
    second = Learner()
    specialterr = []
    for i in [5,10,15,20,30,40]: # A- d=3, w=(5,10,15,20,30,40)
         ret = second.learn2(i,3,train2,test2)
         graph2A(str(i),str(3),ret['trerr'],ret['teerr'],False)
         specialterr.append(ret['teerr'][-1])
    graph2B([5,10,15,20,30,40],3,specialterr)
    # 2 B - snd (with w=10)
    specialterr = []
    for i in range(0,6): # B- w=10, d=(0,1,2,3,4,5)
        ret = second.learn2(10,i,train2,test2)
        graph2A(str(10),str(i),ret['trerr'],ret['teerr'],True)
        specialterr.append(ret['teerr'][-1])
    graph2B(10,range(0,6),specialterr)

def tovect(cls):
    ret = [0]*8
    try:
        x = int(cls)
    except:
        print "Failed\n"
    ret[x-1] = 1
    return ret

def tovect2(clss,cls):
    ret = [0]*len(clss) # go into inner aray
    ret[cls-1] = 1
    return ret
class Learner:

    def __init__(self):
        self.nodes = []
        self.weights = []

    def learn1(self,traindata):
        # Returns array of training error alongside final representation of nodes
        ret = []
        # Construct network with w,d and Initialize weights
        self.nodes = [[0.0]*8]*3 #One-hot style codings
        self.weights = [[list(random.uniform(low=-0.1,high=0.1,size=8)) for i in [0.0]*8],
                   [list(random.uniform(low=-0.1,high=0.1,size=8)) for i in [0.0]*8]]
        for i in range(0,3000): # TODO CHANGE BACK
            errs = 0.0
            for ex in traindata['pts']:
                # Build forward, finding Si, Xi
                s = self.buildS(ex['raw'])
                self.buildX(s) # Saves xis as new nodes
                # Update weights using backpropagation formulas
                # deltan is top row of nodes built off xN w xi as placeholders
                want = tovect(ex['_class'])              
                deltai = self.buildDni(want) # Use class as Y
                self.updateweights(deltai)
                got = [int(round(i)) for i in self.nodes[-1]] # Checks output nodes
                if want != got: # Eg, needs to represent accurately
                    errs += 1
    	    # Calculate the train error rate 
    	    #(number of mistakes during training / number of examples)
            terr = errs/float(len(traindata['pts']))
            ret.append(terr)
        return ret

    def learn2(self,w,d,traindata,testdata):
        # Return 200 times
        # Returns array of training error alongside final representation of nodes
        ret = {'trerr':[],'teerr':[]}
        # Construct network with w,d and Initialize weights
	inputw = len(traindata['pts'][0]['raw'])
	outputw = len(traindata['classes'])
        print "outputw->"+str(traindata['classes'])
        mid = [[0.0]*w]*d
	self.nodes = [[0.0]*inputw]+mid+[[0.0]*outputw]
	# Len d+1 : s.t 0 corresponds to [inputw][w], d corrs to [w][outputw]
        if d==0:
            wei = []
            for i in range(0,inputw):
                wei.append(list(random.uniform(low=-.01,high=0.1,size=outputw)))
            self.weights = [wei] # List of len 1
        else:    
            inwei = []
	    for i in range(0,inputw):
	        inwei.append(list(random.uniform(low=-0.1,high=0.1,size=w)))

	    midwei = [[0.0]*w]*(d-1)        
	    for j in range(0,d-1):
	        for i in range(0,w):
	            midwei[j][i] = list(random.uniform(low=-0.1,high=0.1,size=w))
            outwei = []
	    for i in range(0,w):
	        outwei.append(list(random.uniform(low=-0.1,high=0.1,size=outputw)))
	    self.weights = [inwei] + midwei + [outwei]

        print "weight---"
        for row in self.weights:
            print str(len(row))+"->"+str(len(row[0]))

        for i in range(0,200):#TODO change back
            errs = 0.0
            for ex in traindata['pts']:
                # Build forward, finding Si, Xi
                s = self.buildS(ex['raw'])
                self.buildX(s) # Saves xis as new nodes
                # Update weights using backpropagation formulas
                # deltan is top row of nodes built off xN w xi as placeholders
                want = tovect2(traindata['classes'],ex['_class'])              
                deltai = self.buildDni(want) # Use class as Y
                self.updateweights(deltai)
                got = [int(round(i)) for i in self.nodes[-1]] # Checks output nodes
                if want != got: # Eg, needs to represent accurately
                    errs += 1
    	    # Calculate the train error rate 
    	    #(number of mistakes during training / number of examples)
            terr = errs/float(len(traindata['pts']))
            ret['trerr'].append(terr)
            
            errs = 0.0		
	    # NOW; test on extant trainset
	    for ex in testdata['pts']:
	        # Need to test xi forward
		s = self.buildS(ex['raw'])
		self.buildX(s) # 
	        # Update weights using backpropagation formulas
                # deltan is top row of nodes built off xN w xi as placeholders
                want = tovect2(testdata['classes'],ex['_class'])              
                deltai = self.buildDni(want) # Use class as Y
                self.updateweights(deltai)
                got = [int(round(i)) for i in self.nodes[-1]] # Checks output nodes
                if want != got: # Eg, needs to represent accurately
                    errs += 1
    	    # Calculate the train error rate 
    	    #(number of mistakes during training / number of examples)
            terr = errs/float(len(testdata['pts']))
            ret['teerr'].append(terr)
		
        return ret

    def buildS(self,newx):
        self.nodes[0] = newx
        s = deepcopy(self.nodes) # reuses convenient duplicate of correct arr size
        for j,arr in enumerate(s):
            if j==0: # Not calculating this row (input)
                continue
            for i,val in enumerate(arr): # Goes thru curr nodes
                si = 0.0
                for k,inp in enumerate(s[j-1]): # Goes thru previous nodes
                    temp = inp * (self.weights[j-1][k][i])#weightk,i across all k
                    si += temp
                s[j][i] = si
        return s

    def buildX(self,s):
    	for j,arr in enumerate(s):
            if j==0:
            	continue
            for i,v in enumerate(arr):
            	self.nodes[j][i] = 1.0/(1.0+e**max(-v,-50))

    def buildDni(self,y):
        di = deepcopy(self.nodes)
        for i,xn in enumerate(self.nodes[-1]): #Going through top row of nodes
            di[-1][i] = -(y[i]-xn)*xn*(1-xn)
        # dn are stored in upper component of di array - follows node struct
        for j,arr in enumerate(reversed(di)): # needs reverse
            realj = len(di)-1-j#Actual values - eg, 8,7,6,5,4,3,2,1,0
            for i,v1 in enumerate(arr):
                if j==-1 or j==0:
                    continue # Skips past XN (top node) case and input node case
                sumx = 0.0
                # Constructs: = (sig xi)(1-sigxi) * sum(rents Xi * weight[bw])
                for k,v2 in enumerate(di[realj+1]): # Goes through parent row of nodes
                    sumx += self.weights[realj][i][k] * v2 #weight * successor val
                temp = sumx * sigprime(self.nodes[realj][i])
                di[realj][i] = temp
        return di

    def updateweights(self,deltai):
        for j,row in enumerate(self.weights):
            for i,rents in enumerate(row):
                xj = self.nodes[j][i] # Child node X value
                for k,w in enumerate(rents): # Parent nodes (held in cur arr)
                    x = w - ((LRATE)*(deltai[j+1][k])*xj)
                    if x!=w:
                        self.weights[j][i][k] = x

def sigprime(x):
    sigx = 1.0/(1.0+e**max(-x,-50))
    return sigx*(1-sigx)

def getfile(path):
    df = {'attributes':[],'classes':[],'pts':[]}
    with open(os.getcwd()+'/'+path, 'r') as f:
        for l in f: # goes thru each line
            if len(l) > 10: # ensures no empties
                if l[0]=="@":
                    l = l.split()
                    if l[0]=="@attribute":
                        if l[1]=="class":
                            #https://stackoverflow.com/a/17337613
                            df['classes'] = (map(int,[re.sub('[^0-9]','',i) for i in l[2].split(',')]))
                        else:
                            df['attributes'].append(l[1])
                else:
                    raw = map(int,[re.sub('[^0-9]','',i) for i in l.split(',')])
                    df['pts'].append({'raw':raw[:-1],'_class':raw[-1]})
    return df
###########################################################################
#                           GRAPHING METHODS                              #
########################################################################### 
def graphprep():
    if os.path.exists(os.getcwd()+'/graphs'):
        shutil.rmtree(os.getcwd()+'/graphs')
    # Removed extant
    os.makedirs(os.getcwd()+'/graphs')

def graph1(terr):
    
    #graphprep()    

    # 1st graph: Training error for 1-hot over # features
    x = pd.DataFrame({'iter':range(1,3001),
		              'terr':terr})
    fig, ax = plt.subplots(figsize=(50,5))
    width = 0.3
    plt.plot(x['iter'],x['terr'], linestyle='-', marker='+', label='_nolegend')
    plt.bar(x['iter'],x['terr'], width, alpha=0.5, color='#EE3224', label="Train Error")
    plt.ylim(0.0,1.1)
    plt.title("Training Set Err over Perceptron Iterations")
    plt.ylabel("Training Set Error")
    plt.xlabel("Iteration #")
    plt.show()
    plt.savefig('graphs/terr3000.png')
    plt.gcf().clear()

# Plot w,d errs as function of iterations
def graph2A(w,d,trerr,teerr,second):
    if second:
        fst = "-2"
    else:
        fst = "-1"

    print "tr,teerr----"
    print trerr
    print teerr
    # 2nd graph: d=3 and w is different, over # iterations 
    x = pd.DataFrame({'iter':map(str,range(1,201)),# TODO change back
		              'trerr':trerr,
		              'teerr':teerr})
    # Setting the positions and width for the bars
    pos = list(range(len(x['iter'])))
    width = 0.4

    # Plotting the bars
    fig, ax = plt.subplots(figsize=(40,5))

    plt.bar(pos,x['trerr'], width, alpha=0.5, color='#EE3224', label="Train Error")
    plt.plot(pos,x['trerr'],linestyle='-', marker='+',label='_nolegend_',color='#EE3224')
    plt.bar([p + width for p in pos],x['teerr'],width, alpha=0.5, color='#F78F1E', label="Test Error")
    plt.plot([p + width for p in pos],x['teerr'],linestyle=':',marker='+',label='_nolegend_',color='#F78F1E')

    # Set the position of the x ticks
    ax.set_xticks([p + 1.5 * width for p in pos])

    # Set the labels for the x ticks
    ax.set_xticklabels(x['iter'])

    # Setting the x-axis and y-axis limits
    plt.xlim(min(pos)-width, max(pos)+width*4)
    plt.ylim(0.0,1.3)

    plt.title("Train & Test Set Error over # of Iterations")
    # Set the labels for the x ticks
    plt.ylabel("Training/Test Set Error")
    plt.xlabel("Iteration #")
    # Shrink current axis by 20%
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    # Put a legend to the right of the current axis
    ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
    plt.show()
    plt.savefig("graphs/Aw"+w+"d"+d+fst+".png")

# Plot test error across either ranged d or w (detects)
def graph2B(w,d,terr):
    if type(w) is int:
    	std = "(w="+str(w)+") "
    	x = d # Sets x-axis accessor
    	oer = "over (d="+str(d)+") "
    	xax = "d"
    else:
    	std = "(d="+str(d)+") "
    	x = w
    	oer = "over (w="+str(w)+") "
        xax = "w"
    df = pd.DataFrame({'iter':x,
    	               'terr':terr})
    fig, ax = plt.subplots(figsize=(10,5))
    width = 0.4
    plt.bar(df['iter'],df['terr'], width, alpha=0.5, color='#EE3224', label="Test Error")
    plt.plot(df['iter'],df['terr'],linestyle='-', marker='+',label='_nolegend_',color='#EE3224')

    plt.ylim(0.0,1.1)
    plt.title("Test Set Err"+std+oer+"Perceptron Iterations")
    plt.ylabel("Test Set Error")
    plt.xlabel("Size of "+xax)
    plt.show()
    plt.savefig("graphs/Bover"+xax+".png")

if __name__ == "__main__":
	main()
