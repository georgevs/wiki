# React Context

### Services
```js
// services.js

export default class Services {
  async init() {
    return this;
  }
}
```

### Store
```js
// store.js

export function hydrateState(state, localStorage) {
  return Object.assign(state, {});
}

export function persistState(state, localStorage) {
}

export function reducer(state, action) {
  switch (action.type) {
    case 'LOADING': return { message: 'Loading...', isLoading: true };
    case 'LOAD_ERROR': return { error: action.payload };
    case 'LOADED': return { todos: action.payload };
    default: return state;
  }
}
```

### Services context
```jsx
// ui/services-context.jsx

export const ServicesContext = createContext();

export function ServicesProvider({ children }) {
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

### Store context
```jsx
// ui/store-context.jsx

export const StateContext = createContext();
export const DispatchContext = createContext();

export function StoreProvider({ children }) {
  const [state, dispatch] = useReducer(reducer, {}, (initialState) => {
    return hydrateState(initialState, window.localStorage);
  });
  const ref = useRef();
  
  useEffect(() => {
    ref.current = state;
  }, [state]);
  
  useEffect(() => {
    function commitState() {
      if (ref.current) {
        persistState(ref.current, window.localStorage);
        ref.current = undefined;
      }
    };
    window.addEventListener('beforeunload', commitState);
    document.addEventListener('visibilitychange', commitState);
    return () => {
      window.removeEventListener('beforeunload', commitState);
      document.removeEventListener('visibilitychange', commitState);
      commitState();
    };
  }, []);
  
  return (
    <StateContext.Provider value={state}>
      <DispatchContext.Provider value={dispatch}>
        {children}
      </DispatchContext.Provider>
    </StateContext.Provider>
  );
}
```

### Application
```jsx
// ui/App.jsx

export default function App() {
  return (
    <ServicesProvider>
      <StoreProvider>
        <ToDoPage />
      </StoreProvider>
    </ServicesProvider>
  );
}

function useIo() {
  const services = useContext(ServicesContext);
  const dispatch = useContext(DispatchContext);

  const io = useMemo(() => (!services ? undefined : {
    cancelFetch() {
      this.controller?.abort();
    },
    fetch() {
      const prevController = this.controller;
      this.controller = new AbortController();
      const { signal } = this.controller;
      prevController?.abort();
      dispatch({ type: 'LOADING' });
      services.datasets!.todos.getAll({ signal })
        .then(todos => { !signal.aborted && dispatch({ type: 'LOADED', payload: todos }) })
        .catch(error => { this.controller.signal === signal && dispatch({ type: 'LOAD_ERROR', payload: error }) });
    }
  }), [services, dispatch]);

  return io;
}

function ToDoPage() {
  const state = useContext(StateContext);
  const io = useIo();

  useEffect(() => {
    if (io) {
      io.fetch();
      return io.cancelFetch.bind(io);
    }
  }, [io]);

  const { message, error, todos, isLoading } = state;
  const canceled = error?.name === 'AbortError';

  return (
    <>
      {message && <Message message={message} />}
      {error && !canceled && <Message className='error' message={error.message} />}
      {canceled && <Message className='warning' message={'Canceled!'} />}
      {todos && <ToDosList todos={todos} />}
      <div className='actions'>
        {io && !isLoading && <button onClick={io.fetch.bind(io)}>Fetch</button>}
        {isLoading && <button onClick={io!.cancelFetch.bind(io)}>Cancel</button>}
      </div>
    </>
  );
}

function Message({ className, message }) {
  return (
    <p className={['message', className].join(' ')}>{message}</p>
  );
}

function ToDosList({ todos }) {
  return (
    <ul className='todos'>
      {todos.map(({ id, text }) => (
        <li key={id}>{text}</li>
      ))}
    </ul>
  );
}
```
