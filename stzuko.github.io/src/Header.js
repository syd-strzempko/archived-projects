import React from 'react'
import { Link } from 'react-router-dom'

// The Header creates links that are used to navigate between routes
export default class Header extends React.Component {
	render() {
		return (
		<header>
    		<div className='container elegant-color-dark'>
      			<div className='row'>
        			<Link to='/' className='center-link'>
          				<img className='rounded-circle' src="sydney.jpg" alt="the creator"/>
        			</Link>
      			</div>
      			<div className='row text-center header'>
					<div className='col'>
	  					Syd Strzempko
          				<hr />
					</div>
		    	</div>
		   	</div>
  		</header>
		);
	}
}
