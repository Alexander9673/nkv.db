# nkv.db
An easy to use and fast key-valu database for sqlite3 written in C++ as a NodeJS module.

## What is nkv?
nkv means **N**ative **K**ey **V**alue, which means that the project's code is written in Native C++ and used as a NodeJS module.

## Installation
- Install windows build tools if you don't have it: `npm install windows-build-tools --global`  
- For linux, install `build-essential` with `apt` or `yum`  
- Simply do `npm install nkv.db`

## Usage
**This example does not use the Database class**
```js
const db = require('nkv.db');
db.database("data/db"); // set the database name

db.set('your_key', 'any value'); // add the value "any value" to the database with the identifier 'your_key'
```  

**This example uses the Database class**
```js
const Nkv = require('nkv.db');
const db = new Nkv.Database("table_name", "database_name");

db.set('your_key', 'any value'); // add the value "any value" to the database with the identifier 'your_key'
```