# Backend

## Install live certificates
Fetch certificates:
```bash
openssl enc -aes-128-cbc -pbkdf2 -salt -d -in ~/ws-archive/certs.tar.gz.enc | tar xzv
```
## Manage secrets
Fetch secrets:
```bash
openssl enc -aes-128-cbc -pbkdf2 -salt -d -in ~/ws-archive/secrets.tar.gz.enc | tar xzv
```
Update secrets:
```bash
tar czv secrets | openssl enc -aes-128-cbc -pbkdf2 -salt -out ~/ws-archive/secrets.tar.gz.enc
```
## Run
### Setup a network
```bash
docker network create \
  -d bridge \
  --subnet 172.20.0.0/16 \
  --gateway 172.20.0.1 \
  bridge-app
```
### Run the mysql server
```bash
(read -s -p 'Password:' MYSQL_ROOT_PASSWORD </dev/tty ; \
docker container run \
  --name mysql-app \
  --network bridge-app \
  --ip 172.20.0.201 \
  --volume "$PWD/mysql:/var/lib/mysql" \
  --env MYSQL_ROOT_PASSWORD=$MYSQL_ROOT_PASSWORD \
  --rm -d mysql)

mysql_config_editor set --host=172.20.0.201 --port=3306 --user=root --password
```
### Run the backend app
```bash
docker container run \
  --name node-app \
  --network bridge-app \
  --ip 172.20.0.202 \
  --user node \
  --workdir /home/node \
  --volume "$PWD/app:/home/node/app" \
  --volume "$PWD/certs:/home/node/certs" \
  --volume "$PWD/secrets:/home/node/secrets" \
  --rm -d node index
```
## Backend app code
### Dependencies
```js
const https = require('https');
const express = require('express');
const mysql = require('mysql2');
const cors = require('cors');
const bodyParser = require('body-parser');
const createError = require('http-errors');
const fs = require('fs');
```
### Config
```js
const config = () => ({
  server: {
    key: fs.readFileSync('../certs/cert-key-nopassword.pem', 'utf8'),
    cert: fs.readFileSync('../certs/cert.pem', 'utf8'),
    port: 3443
  },
  db: {
    host: '172.20.0.201', 
    port: 3306, 
    user: 'root', 
    password: process.env.MYSQL_ROOT_PASSWORD
  }
});
```
### Database client
```js
const db = ({ host, port, user, password }) => {
  const connection = mysql.createConnection({ host, port, user, password });
  connection.connect();
  const query = ({ sql, values }, callback) => {
    // console.log('db/query', { sql, values });
    connection.query(sql, values, callback);
  };
  return { query };
};
```
### Router
```js
const users = (db) => {
  const router = express.Router();
  
  router.route('/')
    .get((req, res, next) => {
      const sql = 'SELECT * FROM test.Users';
      db.query({ sql }, (err, results) => {
        if (err) { next(err) }
        else { res.json(results) }
      });
    })
    .post((req, res, next) => {
      const { email, full_name } = req.body;
      const sql = 'INSERT INTO test.Users(email, full_name) VALUES(?,?)';
      const values = [email, full_name];
      db.query({ sql, values }, (err, result) => {
        if (err?.code === 'ER_BAD_NULL_ERROR') { err = createError.BadRequest(err.sqlMessage) }
        else if (err?.code === 'ER_DUP_ENTRY') { err = createError.Conflict(err.sqlMessage) }
        else if (!result?.insertId) { err = createError.InternalServerError(err?.sqlMessage) }
        if (err) { next(err) }
        else { res.json({ id: result.insertId }) }
      });
    })
    .all(() => { throw createError.MethodNotAllowed() });

  router.route('/:id')
    .get((req, res, next) => {
      const id = Number.parseInt(req.params.id);
      if (isNaN(id)) { throw createError.BadRequest('Invalid id') }
      const sql = 'SELECT * FROM test.Users WHERE id=?';
      const values = [id];
      db.query({ sql, values }, (err, results) => {
        if (!results?.length || results[0]?.id !== id) { err = createError.NotFound() }
        if (err) { next(err) }
        else { res.json(results[0]) }
      });
    })
    .delete((req, res, next) => {
      const id = Number.parseInt(req.params.id);
      if (isNaN(id)) { throw createError.BadRequest('Invalid id') }
      const sql = 'DELETE FROM test.Users WHERE id=?';
      const values = [id];
      db.query({ sql, values }, (err, result) => {
        if (!result?.affectedRows) { err = createError.NotFound() }
        if (err) { next(err) }
        else { res.json({ id }) }
      });
    })
    .put((req, res, next) => {
      const id = Number.parseInt(req.params.id);
      if (isNaN(id)) { throw createError.BadRequest('Invalid id') }
      const { email, full_name } = req.body;
      const { terms, values: termsValues } = Array.from(Object.entries({ email, full_name }))
        .filter(([,value]) => value !== undefined)
        .reduce((acc, [key, value]) => (
          acc.terms.push(`${key}=?`),
          acc.values.push(value),
          acc
        ), { terms: [], values: [] });
        
      if (!terms.length) { throw createError.BadRequest('No fields to update') }

      const update = 'UPDATE test.Users';
      const set = ['SET', terms.join(',')].join(' ');
      const where = 'WHERE id=?';

      const sql = [update, set, where].join(' '); 
      const values = [...termsValues, id];

      db.query({ sql, values }, (err, result) => {
        if (!result?.affectedRows) { err = createError.NotFound() }
        if (err?.code === 'ER_BAD_NULL_ERROR') { err = createError.BadRequest(err.sqlMessage) }
        if (err) { next(err) }
        else { res.json({ id }) }
      });
    })
    .all(() => { throw createError.MethodNotAllowed() });

  return router;
};
```
### App
```js
const app = (db) => {
  const app = express();

  app.use(cors());
  app.use(bodyParser.json());
  app.use('/api/v1/users', users(db));
  app.route('*').all(() => { throw createError.NotFound() });

  app.use((err, req, res, next) => {
    // console.log({ err });
    if (err?.sqlMessage) { err = createError.InternalServerError(err.sqlMessage) }
    if (!err?.status) { err = createError.InternalServerError(err) }
    const { status, message } = { ...err, ...err.constructor.prototype };
    res.status(status).json({ status, message });
  });

  return app;
};
```
### HTTPS server
```js
const server = ({ key, cert, port }, app) => {
  const server = https.createServer({ key, cert }, app);
  const start = () => {
    server.listen(port, () => { console.log(`Listening on port ${port}`)});
  };
  return { start };
};

const serve = (config) => {
  server(config.server, app(db(config.db))).start();
};

serve(config());
```
