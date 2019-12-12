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

    const resp = devices.map(({device_id, name}) => {
        return {"id": device_id, "name": name}
    });

    res.status(200).json(resp);
});

app.get('/measurements', async (req, res) => {
    const {device} = req.query;

    const parameters = await db.all("SELECT * FROM measurements WHERE device_id=?", device);

    const resp = {};
    parameters.forEach(({dateline, value}) => {
        resp[dateline] = value;
    });

    res.status(200).json(resp);
});

app.get('/parameters', async (req, res) => {
    const {device} = req.query;

    // Fetch the parameter values for this device.
    const parameters = await db.all("SELECT * FROM parameters WHERE device_id=?", device);

    const resp = {};
    parameters.forEach(({name, value}) => {
        resp[name] = value;
    });

    res.status(200).json(resp);
});

app.put('/parameters', async (req, res) => {
    const {device, parameter, value} = req.body;

    // Update the values for this device.
    await db.run("UPDATE parameters SET value=? WHERE device_id=? AND name=?", value, device, parameter);

    res.status(204).end();
});

app.post('/ping', async (req, res) => {
    const {device, value} = req.body;

    // Update the values for this device.
    await db.run("INSERT INTO measurements (device_id, value) VALUES (?, ?)", device, value);

    // Fetch the parameter values for this device.
    const parameters = await db.all("SELECT * FROM parameters WHERE device_id=?", device);

    // Answer format.
    if (req.accepts("json")) {
        console.log(parameters);
        // Answer in json.
        const resp = {};
        parameters.forEach(({name, value}) => {
            resp[name] = value;
        });
        res.status(201).json(resp);
    } else {
        // Answer literal text.
        const resp = parameters.map(({name, val}) => `${name}=${val}`).join("\n");
        res.status(201).send(resp);
    }
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
