import React from 'react'

export default class Contact extends React.Component {
	render() {
		return (
			<div className='container elegant-color-dark'>
      			<div className='row text-center padded-bottom'>
        			<div className='col-sm'>
          				<h1>Contact</h1>
	  					<span class='text-center icon-wrapper'>
	    					<a href="https://github.com/stzuko">
	      						<i class="fab fa-github-square"></i>
	    					</a>
	    					<a href="https://www.linkedin.com/in/sydney-strzempko-99329abb/">
	      						<i class="fab fa-linkedin"></i>
	    					</a>
            				<a href="mailto:sto.lat.syd@gmail.com">
	      						<i class="fas fa-envelope-square"></i>
	    					</a>
	    					<a href="tel:+15083613887">    
	      						<i class="fas fa-phone-square"></i>
            				</a>
	  					</span>
        			</div>
      			</div>
    		</div>
		);
	}
}
