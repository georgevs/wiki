# Redux

## Install
```
npm install redux redux-thunk
```
## Store
```
import Redux from 'redux'

const store = Redux.createStore(reducer)
const state = store.getState()
store.subscribe(() => { ... })
```
## Middleware
```
import ReduxThunk from 'redux-thunk'  // suport async actions

const store = Redux.createStore(
  reducer,
  Redux.applyMiddleware(ReduxThunk)
)
```
## Action
```
const increment = (delta) => { type: 'INCREMENT', payload: { delta } }
store.dispatch(increment(42))
```
## Async action
```
const fetchData = (url) => (dispatch, getState) => {
  dispatch({ type: 'LOADING' });
  fetch(url)
    .then(data => dispatch({ type: LOADED, payload: { data } }))
    .catch(err => dispatch({ type: LOAD_ERR, payload: { err } }));
}

store.dispatch(fetchData(url))  // requires ReduxThunk middleware
```
## Reducer
```
const initialState = { count: 0 }
const reducer = (state = initialState, action) => {
  switch (action.type) {
    case 'INCREMENT': return { ...state, count: state.count + action.payload.delta };
    default: return state;
  }
}
```
## Combine reducers
```
const rootReducer = Redux.combineReducers({
  auth: authReducer,
  count: counterReducer
})

const store = Redux.createStore(rootReducer)
const { auth: { user }, count } = store.getState()

const authReducer = (state = {}, action) => {
  switch (action.type) {
    case 'LOGIN': return { user: action.payload.user };
    case 'LOGOUT': return {};
    default: return state;
  }
}

const counterReducer = (state = 0, action) => {
  switch (action.type) {
    case 'INCREMENT': return state + action.payload.delta;
    default: return state;
  }
}
```
