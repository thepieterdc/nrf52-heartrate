--------------------------------------------------------------------------------
-- Up
--------------------------------------------------------------------------------

CREATE TABLE parameters
(
    parameter_id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id INTEGER,
    name VARCHAR,
    value DOUBLE,
    CONSTRAINT parameters_device_id_device_id_fk FOREIGN KEY (device_id) REFERENCES devices (device_id)
);

--------------------------------------------------------------------------------
-- Down
--------------------------------------------------------------------------------

DROP TABLE parameters;