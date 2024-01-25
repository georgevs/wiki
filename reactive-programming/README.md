# RxJS

## References
- https://github.com/IliaIdakiev/rxjs-workshop
- https://www.learnrxjs.io
- https://rxmarbles.com
- https://swirly.dev 
- https://jsonplaceholder.typicode.com/users

## Terminology
```js
const observable = new Observable(observer => {
  observer.next(value);
  observer.error(new Error(...));
  observer.complete();
  return function unsubscribe() {...}
});

const observable = creationalOperator(...);
const observable = anotherObservable.pipe( pipeableOperator(...), ... );

const creationalOperator = (...) => new Observable(observer => {...});

const pipeableOperator = (fn) => (inputObservable) => new Observable(outputObserver => {
  const innerSubscription = inputObservable.subscribe({
    next: value => outputObserver.next(fn(value)),
    ...outputObserver
  });
  return function unsubscribe() { innerSubscription.unsubscribe() }
});

const highOrderObservableOperator = pipeableOperator(value => new Observable(...));

const joinOperator = () => (inputObservable) => new Observable(outputObserver => {
  const innerSubscriptions = [
    inputObservable.subscribe({
      next: innerObservable => { innerSubscriptions.push(innerObservable.subscribe(outputObserver)) },
      ...outputObserver
    })
  ];
  return function unsubscribe() { innerSubscriptions.forEach(subscription => subscription.unsubscribe()) }
});

const subscription = observable.subscribe({
  next: value => { ... },
  error: err => { ... },
  complete: () => { ... }
});

subscription.unsubscribe();
```

## Patterns
```js
const delta = base => { let i = 0; return x => `${i++} ${x - base}`};
const log = ((timestamp) => (...xs) => (...ys) => console.log(timestamp(Date.now()), ...xs, ...ys))(delta(Date.now()));

const sink = (...xs) => ({ 
  next: log(...xs, 'next' ), 
  error: log(...xs, 'error'), 
  complete: log(...xs, 'complete') 
});

// synchronous ...
const subscription = rxjs.of(1,2,3)
  .pipe(
    rxjs.tap(sink('debug')),
    rxjs.map(x => x * 10)
  )
  .subscribe(sink('result'))

// asynchronous ...
const throttle = fn => value => new rxjs.Observable(observer => {
  const timer = setTimeout(() => { observer.next(value); observer.complete() }, fn(value));
  return function unsubscribe() { log('unsubscribe')(value); clearTimeout(timer) }
});
const subscription = rxjs.of(1,2,3)
  .pipe(
    rxjs.tap(sink('debug')),
    rxjs.mergeMap(throttle(x => x * 1000))
    // rxjs.switchMap(throttle(x => x * 1000))
    // rxjs.concatMap(throttle(x => x * 1000))
  )
  .subscribe(sink('result'));

subscription.unsubscribe();
```
