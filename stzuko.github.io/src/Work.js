import React from 'react'
import SingleJob from './SingleJob'

export default class Work extends React.Component {
	
	constructor() {
		super();
		this.state = {
			jobs: [
				{'id': 0, 'title':'Full Stack Developer', 'project':'LobbyView Database','link':'https://www.lobbyview.org','img':'lobbyview.png','desc':['Upgraded ElasticSearch version; Rebuilt custom query functions across all indices','Created new word-parsing n-gram cloud visualization; Refactored/repaired current visualizations','Spearheaded the design of a new website with new viewing and query functionality','Overhauled dependency management, Gulp build, and Angular component architecture']},
				{'id': 1, 'title':'Open Source Developer', 'project':'Color Commons Installation','link':'http://www.newamericanpublicart.com/color-commons-2017','img':'ccommons.png','desc':['Built and managed RESTful API architecture for an installation on the Boston Greenway','Gathered and indexed sensitive user information into a queryable format for research','Developed visualization of frequency of user SMS requests with several filter options']},
				{'id': 2, 'title':null, 'project':'Neural Network: Tensorflow', 'link':'https://github.com/stzuko/tfjs-on-mnist', 'img':'cnn.png','desc':['Using tensorflow library, Built and heroku-deployed a webapp with a Nodejs backend to host a convolutional neural network','Prepared and trained the model on data personally constructed from a set of images','Predicted digit class on user-inputted canvas drawings with a React frontend']},
				{'id': 3, 'title':null, 'project':'Neural Network: Handmade','link':'https://github.com/stzuko/neural-network','img':'nn.png','desc':['Built a neural network from scratch to run with multi-depth/width settings on nonbinary features','Graphed test set classifier accuracy to portray best depth and width combinations for learning']},
				{'id': 4, 'title':null, 'project':'Classifier Comparison','link':'https://github.com/stzuko/IBK-v-J48','img':'knn.png','desc':['Demonstrated iterable kNN and decision tree learning on multi-feature classification sets','Graphed best test set accuracy as a product of variation in features vs train set size']},
				{'id': 5, 'title':null, 'project':'Universal Machine','link':'https://github.com/stzuko/universal-machine','img':'um.png','desc':['Coded a fully functional (13-command) UM with segmented memory in C as part of a paired team','Tested refactored code against progressively better benchmarks with GDB/DDB debugging']},
			]
		}
	}
	
	render() {
		return <div className='container elegant-color-dark'>
			{(this.state.jobs).map(job=>(<SingleJob job={job} key={job.id} />))}
		</div>
	}
}
