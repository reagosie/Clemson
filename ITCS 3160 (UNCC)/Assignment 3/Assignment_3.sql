USE prints;
/* #1 */
CREATE VIEW Under_100 AS SELECT items.item_id, title, artist, unit_price, order_qty FROM items, orderline WHERE unit_price < 100;

/* #2 */
CREATE VIEW Allen AS SELECT customer_id, customer_name, customer_phone, title, artist FROM customers, items;

/* #3 */
CREATE VIEW orders_view AS SELECT items.item_id, title, artist, unit_price, order_qty FROM items, orderline, orders WHERE order_date BETWEEN 2014-01-01 AND 2014-02-28;

/* #4 */
CREATE VIEW zip_27 AS SELECT customer_name, customer_phone, title, artist, ship_date FROM customers, items, orders WHERE customer_zip LIKE "27%";

/* #5 */
CREATE INDEX customer_id ON customers (customer_id);
CREATE INDEX name ON customers (customer_name);
CREATE INDEX shipped ON orders (customer_id, ship_date);

/* #6 */
DROP INDEX name ON customers;

/* #7 */
ALTER TABLE items ADD CHECK (unit_price > 35);

/* #8 */
ALTER TABLE orders ADD FOREIGN KEY (customer_id) REFERENCES customers(customer_id);
/* Needed to give item_id a unique constraint in order to make it a foreign key in the orderline table*/
ALTER TABLE items ADD UNIQUE (item_id);
ALTER TABLE orderline ADD FOREIGN KEY (item_id) REFERENCES items(item_id);

/* #9 */
ALTER TABLE items ADD type CHAR(1);

/* #10 */
UPDATE items SET type = 'M' WHERE title = 'Skies Above';

/* #11 */
ALTER TABLE items MODIFY COLUMN artist CHAR(30);

/* #12 */
/* the following command would delete the orders table but I have commented it out*/
#DROP TABLE orders;
