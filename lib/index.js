const { existsSync, mkdirSync } = require('fs');
let db = require('../src/build/Release/nkv.db');

function table(name) {
  db.table(name);
}

function set(key, value) {
  db.set(key, value);
}

function get(key) {
  let res = db.get(key);

  try {
    res = JSON.parse(res);
  } catch(e) {}

  if (!isNaN(res)) res = parseInt(res);

  return res;
}

function all() {
  let res = db.all();

  for (let r of res) {
    if (!isNaN(r.data))
      r.data = parseInt(r.data);
    else {
      try {
        r.data = JSON.parse(r.data);
      } catch(e) {}
    }
  }

  return res;
}

function has(key) {
  return db.has(key);
}

function remove(key) {
  return db.delete(key);
}

function database(name) {
  name = !name.endsWith(".sqlite") ? `${name}.sqlite` : name;
  if (name.includes('/') || name.includes('\\') || name.includes('\\\\')) {
    let locations = name.split(/\/|\\\\|\\/g);
    let dir = locations.slice(0, locations.length - 1).join('/');
  
    if (!existsSync(dir))
      mkdirSync(dir, { recursive: true });
  }

  return db.database(process.cwd() + '/' + name);
}

class Database {
  constructor(table_name, database_name) {
    this.table_name = table_name;
    this.db = require('../src/build/Release/nkv.db')
    
    if (this.table_name)
      this.table(table_name);

    if (database_name) {
      database_name = database_name && !database_name.endsWith(".sqlite") ? `${database_name}.sqlite` : database_name;
      if (database_name.includes('/') || database_name.includes('\\') || database_name.includes('\\\\')) {
        let locations = database_name.split(/\/|\\\\|\\/g);
        let dir = locations.slice(0, locations.length - 1).join('/');
  
        if (!existsSync(dir))
          mkdirSync(dir, { recursive: true });
      }
    
      this.db.database(process.cwd() + '/' + database_name || "quickdb.sqlite");
    }
  }

  table(table_name) {
    return table(table_name)
  }

  set(key, value) {
    return set(key, value);
  }

  get(key) {
    return get(key);
  }
  
  all() {
    return all();
  }

  has(key) {
    return has(key);
  }

  remove(key) {
    return remove(key);
  }
}

module.exports = {
  Database,
  table,
  set,
  get,
  all,
  has,
  remove,
  database
};