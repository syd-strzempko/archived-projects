import React from 'react'

export default class Art extends React.Component {
	
	constructor(){
		super();
		this.state = {
			max: 29,
			carousel: null,
		}
	}

	componentDidMount() {
		const list = [];
		for (let i=this.state.max;i>0;i--) {
			let link = '/img/art/' + ('0000'+i).slice(-4) + '.jpg';
			// We declare image and set its source as a kind of quickmount of all images
			let img = document.createElement('img');
			img.src = link;
			let panel = (<div className="art-wrapper"><a href={link} target="_blank"><img src={link} alt="weird watercolor"></img></a></div>);
			list.push(panel);
		}
		this.setState({carousel:list});
	}

	render() {	
		return ( 
    		<div className='container elegant-color-dark'>
				<div className='row'>
					<div id="carousel-wrapper" className='col text-center'>{this.state.carousel}</div>
				</div>
				<div className='row'>
					<div className='col text-center'>Click to expand image</div>
				</div>
			</div>
		);
	}
}
