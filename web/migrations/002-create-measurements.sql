--------------------------------------------------------------------------------
-- Up
--------------------------------------------------------------------------------

CREATE TABLE measurements
(
    measurement_id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id INTEGER,
    value DOUBLE,
    dateline INTEGER DEFAULT (strftime('%s', 'now')),
    CONSTRAINT logs_device_id_device_id_fk FOREIGN KEY (device_id) REFERENCES devices (device_id)
);

--------------------------------------------------------------------------------
-- Down
--------------------------------------------------------------------------------

DROP TABLE measurements;