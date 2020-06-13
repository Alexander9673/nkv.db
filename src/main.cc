#include <iostream>
#include "napi.h"
#include "./include/sqlite3.h"

using namespace std;
using namespace Napi;

string db_name;
string table_name = "data";
sqlite3* db;

void __init_sqlite() {
  int rc;
  string sql;
  sqlite3_stmt* stmt;

  rc = sqlite3_open(db_name.empty() ? "nkv.sqlite" : db_name.c_str(), &db);

  if (rc != SQLITE_OK) {
    cout << "can't open database" << endl;
    exit(0);
  }

  sql = "CREATE TABLE IF NOT EXISTS " + (table_name.empty() ? "data" : table_name) + "(" \
    "ID text primary key," \
    "json text);";

  rc = sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);

  if (rc != SQLITE_OK) {
    cout << sqlite3_errmsg(db) << endl;
    exit(0);
  }

  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  sql = "PRAGMA synchronous = OFF;";
  rc = sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
  sqlite3_step(stmt);

  sqlite3_finalize(stmt);

  sql = "PRAGMA journal_mode = WAL;";
  rc = sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}

void __set_data(const CallbackInfo& info) {
  if (db == nullptr) __init_sqlite();

  Env env = info.Env();
  string key = info[0].As<String>().Utf8Value();

  int rc;
  string sql;
  sqlite3_stmt* stmt;
  string string_val;
  bool is_null = false;

  // handle values first
  if (info[1].IsNull() || info[1].IsUndefined()) // is null or undefined
    is_null = true;
  else if (info[1].IsObject()) { // is obj
    Object JSON = env.Global().Get("JSON").As<Object>();
    Function stringify = JSON.Get("stringify").As<Function>();

    string_val = stringify.Call(JSON, { info[1] }).As<String>().Utf8Value();
  } else if (info[1].IsNumber())
    string_val = info[1].ToString().Utf8Value();
  else string_val = info[1].As<String>().Utf8Value();

  sql = "INSERT OR REPLACE INTO " + (table_name.empty() ? "data" : table_name) + " (" \
    "ID, JSON)" \
    " VALUES (?, " + (is_null ? "NULL" : "?") + ");";

  rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
  rc = sqlite3_bind_text(stmt, 1, key.c_str(), strlen(key.c_str()), NULL);

  if (!is_null)
    rc = sqlite3_bind_text(stmt, 2, string_val.c_str(), strlen(string_val.c_str()), NULL);

  if (rc != SQLITE_OK) {
    cout << sqlite3_errmsg(db) << endl;
    exit(0);
  }

  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

Value __get_data(const CallbackInfo& info) {
  if (db == nullptr) __init_sqlite();

  Env env = info.Env();
  string key = info[0].As<String>().Utf8Value();
  Value data;

  int rc;
  string sql;
  sqlite3_stmt* stmt;

  sql = "SELECT json FROM " + (table_name.empty() ? "data" : table_name) + " WHERE ID = ?";
  rc = sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
  rc = sqlite3_bind_text(stmt, 1, key.c_str(), strlen(key.c_str()), NULL);

  if (rc != SQLITE_OK) {
    cout << sqlite3_errmsg(db) << endl;
    exit(0);
  }

  while(sqlite3_step(stmt) == SQLITE_ROW) {
    if (sqlite3_column_type(stmt, 0) == SQLITE_NULL)
      data = env.Null();
    else {
      string result((char*)sqlite3_column_text(stmt, 0));
      data = String::New(env, result);
    }
  }

  sqlite3_finalize(stmt);

  return data;
}

Value __get_all_data(const CallbackInfo& info) {
  if (db == nullptr) __init_sqlite();

  Env env = info.Env();
  Value data = Array::New(env);
  int arr_pos = 0;

  int rc;
  string sql;
  sqlite3_stmt* stmt;

  sql = "SELECT * FROM " + (table_name.empty() ? "data" : table_name);
  rc = sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);

  if (rc != SQLITE_OK) {
    cout << sqlite3_errmsg(db) << endl;
    exit(0);
  }

  while(sqlite3_step(stmt) == SQLITE_ROW) {
    Object obj_data = Object::New(env);
    obj_data.Set(String::New(env, "index"), Number::New(env, arr_pos));
    obj_data.Set(String::New(env, "key"), string((char*)sqlite3_column_text(stmt, 0)));

    if (sqlite3_column_type(stmt, 1) == SQLITE_NULL)
      obj_data.Set(String::New(env, "data"), env.Null());
    else obj_data.Set(String::New(env, "data"), String::New(env, string((char*)sqlite3_column_text(stmt, 1))));

    data.As<Array>().Set(arr_pos, obj_data);
    arr_pos++;
  }

  sqlite3_finalize(stmt);

  return data;
}

Value __has_data(const CallbackInfo& info) {
  if (db == nullptr) __init_sqlite();

  Env env = info.Env();
  string key = info[0].As<String>().Utf8Value();
  string sql;
  int rc;
  sqlite3_stmt* stmt;

  sql = "SELECT * FROM " + (table_name.empty() ? "data" : table_name) + " WHERE ID = ?";
  rc = sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
  rc = sqlite3_bind_text(stmt, 1, key.c_str(), strlen(key.c_str()), NULL);

  if (rc != SQLITE_OK) {
    cout << sqlite3_errmsg(db) << endl;
    exit(0);
  }

  int count = 0;

  while(sqlite3_step(stmt) == SQLITE_ROW)
    count++;

  sqlite3_finalize(stmt);

  if (count > 0)
    return Boolean::New(env, true);
  else return Boolean::New(env, false);
}

Boolean __delete_data(const CallbackInfo& info) {
  if (db == nullptr) __init_sqlite();

  Env env = info.Env();
  string key = info[0].As<String>().Utf8Value(); 
  string sql;
  sqlite3_stmt* stmt;
  int rc;

  sql = "DELETE FROM " + (table_name.empty() ? "data" : table_name) + " WHERE ID = ?";
  rc = sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
  rc = sqlite3_bind_text(stmt, 1, key.c_str(), strlen(key.c_str()), NULL);

  if (rc != SQLITE_OK) {
    cout << sqlite3_errmsg(db);
    exit(0);
  }

  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  return Boolean::New(env, sqlite3_changes(db) > 0 ? true : false);
}

void __set_table_name(const CallbackInfo& info) {
  table_name = info[0].As<String>().Utf8Value();
}

void __change_database(const CallbackInfo& info) {
  db_name = info[0].As<String>().Utf8Value();
  __init_sqlite();
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "set"), Function::New(env, __set_data));
  exports.Set(String::New(env, "get"), Function::New(env, __get_data));
  exports.Set(String::New(env, "all"), Function::New(env, __get_all_data));
  exports.Set(String::New(env, "has"), Function::New(env, __has_data));
  exports.Set(String::New(env, "delete"), Function::New(env, __delete_data));
  exports.Set(String::New(env, "table"), Function::New(env, __set_table_name));
  exports.Set(String::New(env, "database"), Function::New(env, __change_database));
  return exports;
}

NODE_API_MODULE(nkvdb, Init)