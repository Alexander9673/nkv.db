const db = require('./lib/index');
const quickdb = require('quick.db');

db.table('data');

console.time('set_rewrite_c++')
for (let i = 0; i < 900; i++) {
  db.set(i.toString(), { index: i });
}
console.timeEnd('set_rewrite_c++');
console.time('quickdb_normal')
for (let i = 0; i < 900; i++) {
  quickdb.set(i.toString(), { index: i });
}
console.timeEnd('quickdb_normal');