let db = require('../src/build/Release/quick.db');

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

class Table {
  constructor(table_name) {
    this.table_name = table_name;
    this.db = require('../src/build/Release/quick.db');
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
  Table,
  table,
  set,
  get,
  all,
  has,
  remove
};