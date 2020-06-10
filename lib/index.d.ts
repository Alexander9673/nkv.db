/**
 * Sets the table name
 * @param name The name of the table to use
 */
declare function table(name: string): void;

/**
 * Add any data to the table with an identifier
 * @param key The key or identifier for the data
 * @param value The data to insert
 */
declare function set(key: string, value: any): void;

/**
 * Fetches data from the table with its identifier
 * @param key The identifier for the data to fetch
 */
declare function get(key: string): any;

/**
 * Returns all the identifiers and its values in an array
 */
declare function all(): any[];

/**
 * Check if a a pair of key/value exists
 * @param key The identifier to check
 */
declare function has(key: string): boolean;

/**
 * Removes a pair of key/value from the database
 * @param key The identifier to use to find the pair to remove
 */
declare function remove(key: string): boolean;

/**
 * The quick.db class, it contains the same methods as the previous ones. Can be used to initiate/create a new table instead of .table
 * @param table_name The name of the table to use
 */
declare class Table {
  constructor(table_name: string);
  /**
   * Sets the table name
   * @param name The name of the table to use
   */
  public table(name: string): void;

  /**
   * Add any data to the table with an identifier
   * @param key The key or identifier for the data
   * @param value The data to insert
   */
  public set(key: string, value: any): void;

  /**
   * Fetches data from the table with its identifier
   * @param key The identifier for the data to fetch
   */
  public get(key: string): any;

  /**
   * Returns all the identifiers and its values in an array
   */
  public all(): any[];

  /**
   * Check if a a pair of key/value exists
   * @param key The identifier to check
   */
  public has(key: string): boolean;

  /**
   * Removes a pair of key/value from the database
   * @param key The identifier to use to find the pair to remove
   */
  public remove(key: string): boolean;
}

export const _default: {
  set: typeof set;
  get: typeof get;
  all: typeof all;
  has: typeof has;
  remove: typeof remove;
  Table: typeof Table;
}