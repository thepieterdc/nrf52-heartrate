--------------------------------------------------------------------------------
-- Up
--------------------------------------------------------------------------------

INSERT INTO parameters (name, device_id, value) VALUES ('max', 1, 80);
INSERT INTO parameters (name, device_id, value) VALUES ('min', 1, 45);

--------------------------------------------------------------------------------
-- Down
--------------------------------------------------------------------------------

DELETE FROM parameters;