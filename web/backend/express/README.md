# Express

## Instantiate
```
// npm install express
const express = require('express');
const app = express();
```
## Configure environment
Put configuration in `.env` file
```
PORT=3443
KEY_PEM=.../secret/key.pem
CERT_PEM=.../secret/cert.pem
```
Load environment with `dotenv`
```
// npm install dotenv
const dotenv = require('dotenv');
dotenv.config();
const { PORT, KEY_PEM, CERT_PEM } = process.env;
```
## Middleware
```
app.use((req, resp, next) => {
  console.log(req.method, req.path, req.ip);
  next();
})
```
## Route
```
const handler = (req, resp) => { ... };
app.get(path, handler);
app.post(path, handler);
app.route(path).get(handler).post(handler)...
```
## Serve static content in folder
```
app.use(express.static(localFolderPath));
```
## Respond with file contents
```
const handler = (req, resp) => { resp.sendFile(localFilePath) };
```
## Respond with string
```
const handler = (req, resp) => { resp.send('...') };
```
## Respond with json
```
const handler = (req, resp) => { resp.json({ key: value, ... }) };
```
## Params in route
```
// https://example.com/user/123/book/456
app.get(
  '/user/:userId/book/:bookId', 
  (req, resp) => { const { userId, bookId } = req.params; ... }
)
```
## Params in query
```
// https://example.com/details?user=123&book=456
app.get(
  '/details',
  (req, resp) => { const { user: userId, book: bookId } = req.query; ... }
)
```
## Params in body (url encoded)
```
// npm install body-parser
const bodyParser = require('body-parser');
app.use(bodyParser.urlencoded());

// <form method='post' action='details'>
//    <input name='user'...>... 
//    <input name='book' ...>...
app.get(
  '/details',
  (req, resp) => { const { user: userId, book: bookId } = req.body; ... }
)
```
## Params in body (json)
```
// npm install body-parser
const bodyParser = require('body-parser');
app.use(bodyParser.json());

// <script>
//    fetch('/details', { method: 'POST', body: JSON.stringify({ name, book })})
app.get(
  '/details',
  (req, resp) => { const { user: userId, book: bookId } = req.body; ... }
)
```
## Middleware in request
```
app.get(
  '/now',
  (req, resp, next) => { req.time = new Date(); next() },
  (req, resp) => { const { time } = req; ... }
)
```
## Handling errors
```
const express = require('express');
const app = express();
const createError = require('http-errors');

app.get('/users/:id', (req, res, next) => {
  const id = Number.parseInt(req.params.id);
  if (isNaN(id)) { next(createError.BadRequest()); return } // or throw(createError.BadRequest())
  
  db.query({ sql, values }, (err, results) => {
    if (!results.length) { err = createError.NotFound() }
    if (err?.code === 'ER_DUP_ENTRY') { err = createError.Conflict(err.sqlMessage) }

    if (err) { next(err) }
    else { res.json(results) }
  });
});

app.use((err, req, res, next) => {
    if (err?.sqlMessage) { err = createError.InternalServerError(sqlMessage) }
    if (!err?.status) { err = createError.InternalServerError(err) }
    const { status, message } = { ...err, ...err.constructor.prototype };
    res.status(status).json({ status, message });
});
```