import React from 'react'

export default class SingleJob extends React.Component {
        constructor(props) {
                super(props);
                this.state = {
                        expanded: false
                }
        }

        toggleCollapse(){
                this.setState({expanded: (this.state.expanded? false : true)})
        }

	computeButton(){
		return (this.state.expanded? <i className="fas fa-caret-up"></i> : <i className="fas fa-caret-down"></i>)
	}

        render() {
          return (
                <div className='container elegant-color-dark job-wrapper text-center' >
                  	<Title title={this.props.job.title} project={this.props.job.project}/>
                  	<Image src={this.props.job.img} />
					<button className='show-more' onClick={this.toggleCollapse.bind(this)}>{this.computeButton()}</button>
                  	{this.state.expanded && <Collapsible text={this.props.job.desc} link={this.props.job.link} />}
                </div>
          );
        }
}

function Title(props){
  return (
    <div className="container">
      <div className="row text-center">
        <div className="col">
          <h1>{props.title}</h1>
        </div>
      </div>
      <div className="row text-center">
        <div className="col">
          <h2>{props.project}</h2>
        </div>
      </div>
    </div>
  )
}

function Image(props){
  return (
          <div className='container img-container'>
                <img src={'/img/work/' + props.src} alt="screenshot of hosted" className="work-img"/>
          </div>
  )
}

function Collapsible(props){
  let built = [];
  for (let i=0;i<props.text.length;i++){
	built.push(<li>{props.text[i]}</li>);
  }
  return (
    <div className="container">
      <div className="row">
        <div className="col text-center">
        	<a href={props.link}><i className="fas fa-link"></i></a>
			<ul className="text-left">{built}</ul>
        </div>
      </div>
    </div>
  )
}
