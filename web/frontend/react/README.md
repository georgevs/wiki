# React

## Design
![react-redux](./react-redux.drawio.svg)

## Render
In client (aka front-end)
```
ReactDOM.render(instance, document.getElementById(id))
```
In server (aka back-end)
```
RenderDOMServer.renderToString(instance)
```
## Instantiate
```
const instance = <div>...</div>;
const instance = <Component name='value' name2={value} />;
```
## Stateless functional component
```
const Component = (props) => <div>{props.name}</div>;
```
## Stateless component
```
class Component extend React.Component {
  constructor(props) { super(props) }
  render() { return (<div>{props.name}</div>) }
}
```
## Default properties and properties types
```
Component.defaultTypes = { name: 'value', name2: value }
Component.propTypes = { name: PropTypes.string.isRequired, name2: ... }
```
## State
```
class Component extends React.Component {
  constructor(props) {
    super(props);
    this.state = { count: 0, ... }   <--- init
  }
  increment() { 
    this.setState({ count: 42 })   <--- reset
    this.setState(prevState => ({ count: prevState.count + 1 }))   <--- update
  }
  render() {
    return (<div>{this.state.count}</div>)
  }
}
```
## Event handlers
```
class Component extends React.Component {
  constructor(props) {
    super(props);
    this.state = { email: '' };
    this.handleChange = this.handleChange.bind(this);   <--- bind `this`
  }
  handleChange(event) {
    const { type, name, value, checked } = event.target;
    this.setState({ [name]: type === 'checkbox' ? checked : value });
  }
  render() {
    return (
      <form>
        <input type='text' name='email' value={this.state.email} onChange={handleChange} />
      </form>
    )
  }
}
```
## Lifecycle
```
class Component extends React.Component {
  constructor(props) { super(props) }

  componentWillMount() { ... }    <--- deprecated
  
  componentDidMount() {
    fetch(url).then(data => this.setState({ data }));
    document.addEventListener(...);
  }

  shouldComponentUpdate(nextProps, nextState) { ... return true }
  componentDidUpdate() { ... }

  render() { ... }
  
  componentWillUnmount() { 
    document.removeEventListener(...);
  }
}
```