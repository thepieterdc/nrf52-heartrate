"use strict";

const bodyParser = require('body-parser');
const db = require('sqlite');
const express = require('express');
const path = require('path');
const Promise = require('promise');

const app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
    res.status(200).render('index.html');
});

app.get('/devices', async (req, res) => {
    const devices = await db.all("SELECT * FROM devices");

    const resp = devices.map(({device_id, address}) => {
        return {"id": device_id, "address": address}
    });

    res.status(200).json(resp);
});

app.get('/measurements', async (req, res) => {
    const {device} = req.query;

    // Find the device id for the given address.
    const {device_id} = await db.get("SELECT device_id FROM devices WHERE address=?", device);

    const parameters = await db.all("SELECT * FROM measurements WHERE device_id=? ORDER BY dateline DESC", device_id);

    const resp = {};
    parameters.forEach(({dateline, value}) => {
        resp[dateline] = value;
    });

    res.status(200).json(resp);
});

app.get('/parameters', async (req, res) => {
    const {device} = req.query;

    // Find the device id for the given address.
    const {device_id} = await db.get("SELECT device_id FROM devices WHERE address=?", device);

    // Fetch the parameter values for this device.
    const parameters = await db.all("SELECT * FROM parameters WHERE device_id=?", device_id);

    const resp = {};
    parameters.forEach(({name, value}) => {
        resp[name] = value;
    });

    res.status(200).json(resp);
});

app.put('/parameters', async (req, res) => {
    const {device, parameter, value} = req.body;

    // Find the device id for the given address.
    const {device_id} = await db.get("SELECT device_id FROM devices WHERE address=?", device);

    if (value < 30 || value > 200) {
        res.status(400).end();
    } else {
        // Update the values for this device.
        await db.run("UPDATE parameters SET value=? WHERE device_id=? AND name=?", value, device_id, parameter);

        res.status(204).end();
    }
});

app.post('/ping', async (req, res) => {
    const {device, measurements} = req.body;

    console.log(req.body);

    // Find the device id for the given address.
    const {device_id} = await db.get("SELECT device_id FROM devices WHERE address=?", device);

    // Query placeholders.
    const placeholders = measurements.map((_) => `(${device_id}, ?, ?)`).join(',');

    // Create the query.
    const sql = `INSERT INTO measurements (device_id, dateline, value) VALUES ${placeholders}`;

    // Find the parameter values.
    const parameter_values = measurements.map(({timestamp, value}) => [timestamp, value]).flat();

    // Execute the query.
    const resp = await db.run(sql, parameter_values);

    res.status(204).end();
});

Promise.resolve()
    .then(() => db.open(process.env['DATABASE'], {Promise}))
    .then(() => db.migrate({force: 'last'}))
    .catch((err) => console.error(err.stack))
    .finally(() => {
        app.listen(process.env['PORT'], '0.0.0.0', () => {
            console.info(`Started the server at port: ${process.env['PORT']}`);
        });
    });
