# MongoDB and Mongoose

## References
[freeCodeCamp MongoDB](https://www.freecodecamp.org/learn/back-end-development-and-apis/mongodb-and-mongoose)

[Mongoose](https://mongoosejs.com)

## MongoDB Atlass account
[Mongo DB Atlass](https://cloud.mongodb.com/) is MongoDB as SaaS
```
openssl enc -aes-128-cbc -pbkdf2 -salt -e -in .env -out .env.enc
openssl enc -aes-128-cbc -pbkdf2 -salt -d -in .env.enc -out .env
  z1p#
```
## Docker mongo image (optional)
```
docker image pull mongo
```
## Install mongo
```
// npm install mongodb mongoose
const mongoose = require('mongoose');
```
## Connect database
```
require('dotenv').config({ path: '.env' });
mongoose.connect(process.env.MONGO_URI, { useNewUrlParser: true, useUnifiedTopology: true });
```
## Define schema
```
const personSchema = new mongoose.Schema({
  name: { type: String, required: true },
  age: Number,
  favoriteFoods: [String]
});
```
## Schema types
```
// https://mongoosejs.com/docs/schematypes.html
...Schema({
  ofString: [String],
  ofMixed: [Schema.Types.Mixed],
  ...
})
```
## Virtual fields
```
personSchema.virtual('fullName')
  .get(function getter() { return `${this.firstName} ${this.lastName}` })
  .set(function setter(value) { ([this.firstName, this.lastName] = value.split(' ')) })

console.log(person.fullName)
person.fullName = 'Joe Dow'
```
## Methods
```
personSchema.methods.getInitials = function () { return ... }
console.log(person.getInitials())
```
## Static methods
```
personSchema.statics.getUser = function () { return { ... } }
console.log(Person.getUser().then(person => ...))
```
## Middleware
```
personSchema.pre('save', function (next) {
  this.updateTimestamp = new Date();   // <--- decorate record with synthetic fields
  next();  // <--- continue `save` flow
})
```
## Define model
```
const Person = mongoose.model('Person', personSchema);
```
## Create
```
const record = new Person({ name, age })
record.save().then(person => { ... })
```
## Read
```
Person.find({ ...person })
  .then(person => { ... })
  .catch(err => { ... })

Person.findOne({ ...lookupFields })...  
Person.findById(person._id)...  
```
## Update
```
const person = Person.findOneAndUpdate(
  { ...lookupFields }, 
  { ...updatedFields },
  { new: true },       // <--- options: return the updated document
  done    // <--- call `done()` callback at the end
);
```
## Delete
```
Person.findByIdAndRemove(person._id)...;
Person.remove({ ...lookupFields })...;
```
## Chain
```
Person.find({ favoriteFoods: { $all: [foodToSearch] } })   // <--- `$all` filter operations
  .sort({ name: 1 })
  .limit(2)
  .select({ age: false })
  .exec(done);   // <--- call `done()` callback at the end
```
