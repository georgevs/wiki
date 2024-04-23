# React Context

### Services
```js
// services.js

class Services {
  async init() {
    // connect to db, etc.
  }
}
```

### Services context
```jsx
// services-context.jsx

const ServicesContext = createContext();

function ServicesProvider({ children }) {
  const [services, setServices] = useState();
  
  useEffect(() => {
    let canceled = false;
    new Services().init().then(services => { !canceled && setServices(services) });
    return () => { canceled = true }  
  }, []);

  return (
    <ServicesContext.Provider value={services}>
      {children}
    </ServicesContext.Provider>
  );
}
```

### Store
```js
// store.js

function hydrateState(localStore) {
  return {};
}

function persistState(state, localStore) {
}

function reducer(state, action) {
  switch (action.type) {
    case 'LOADING': return { message: 'Loading...' };
    case 'LOAD_ERROR': return { error: action.payload };
    case 'LOADED': return { users: action.payload };
    default: return state;
  }
}
```

### Store context
```jsx
// store-context.jsx

const StateContext = createContext();
const DispatchContext = createContext();

const initialState = hydrateState(window.localStore);

function StoreProvider({ children }) {
  const [state, dispatch] = useReducer(reducer, initialState);

  useEffect(() => {
    return () => { persistState(state, window.localStore) };
  }, []);
  
  return (
    <StateContext.Provider value={state}>
      <DispatchContext.Provider value={dispatch}>
        {children}
      </DispatchContext>
    </StateContext>
  );
}
```

### Application
```jsx
// App.jsx

function App() {
  return (
    <ServicesProvider>
      <StoreProvider>
        <XComponent />
      </StoreProvider>
    </ServicesProvider>
  );
}
```

### X Component
```jsx
// XComponent.jsx

function XComponent() {
  const services = useContext(ServicesContext);
  const state = useContext(StateContext);
  const dispatch = useContext(DispatchContext);

  useEffect(() => {
    let canceled = false;
    const controller = new AbortController();
    if (services) {
      dispatch({ type: 'LOADING' });
      services.db.users.get({ signal: controller.signal })
        .then(users => { !canceled && dispatch({ type: 'LOADED', payload: users }) })
        .catch(error => { !canceled && dispatch({ type: 'LOAD_ERROR', payload: error }) });
    }
    return () => { canceled = true; controller.abort() }
  }, [services]);
  
  const { message, error, users } = state;
  
  return (
    <>
      {message && <Message message={message} />}
      {error && <Error error={error} />}
      {users && <Users users={users} />}
    </>
  );
}
```
