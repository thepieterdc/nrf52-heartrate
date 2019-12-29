"use strict";

const device_select = '#device-select';
const measurements_chart = '#measurements-chart';
const measurements_tbl = '#measurements-tbl';
const parameters_tbl = '#parameters-tbl';
const parameter_input = 'parameter-input';

let chart = null;

async function changeParameter(device, parameter, value) {
    try {
        const resp = await $.ajax({
            data: {device: device, parameter: parameter, value: value},
            dataType: 'json',
            type: 'PUT',
            url: `/parameters`
        });

        return true;

    } catch (ex) {
        return false;
    }
}

async function loadDevice(device) {
    await loadMeasurements(device);
    await loadParameters(device);
}

async function loadDevices() {
    const $device_select = $(device_select);

    const resp = await $.getJSON(`/devices`);

    $device_select.html(Object.values(resp).map(({id, address}) => {
        return `<option value="${id}" data-address="${address}">${address}</option>`;
    }));
}

async function loadMeasurements(device) {
    const $measurements_chart = $(measurements_chart);
    const $measurements_tbl = $(measurements_tbl);

    const resp = await $.getJSON(`/measurements?device=${device}`);

    if (chart !== null) {
        chart.destroy();
    }

    if (Object.keys(resp).length === 0) {
        $measurements_tbl.html(`
            <tr>
                <td colspan="2" style="text-align: center">
                    <i>No measurements yet.</i>
                </td>
            </tr>
        `);
    } else {
        $measurements_tbl.html(Object.entries(resp).slice(0, 5).map((entry) => {
            const [dateline, value] = entry;
            return `
                <tr>
                    <td>${parseTimestamp(dateline)}</td>
                    <td>${parseFloat(value)}</td>
                </tr>    
            `;
        }));

        const xs = [];
        const ys = [];

        Object.entries(resp).forEach((entry) => {
            xs.push(new Date(parseInt(entry[0])));
            ys.push(entry[1]);
        });

        chart = new Chart($measurements_chart, {
            data: {
                datasets: [{
                    backgroundColor: '#4582EC',
                    borderColor: '#4582EC',
                    borderWidth: 1,
                    data: ys,
                    fill: false,
                    label: 'Measured heart rate'
                }],
                labels: xs
            },
            options: {
                scales: {
                    xAxes: [{
                        type: 'time',
                        time: {stepSize: 5, unit: 'second'}
                    }],
                    yAxes: [{
                        ticks: {
                            min: Math.min(...ys) * 0.995
                        }
                    }]
                }
            },
            type: 'line'
        });
    }
}

async function loadParameters(device) {
    const $parameters_tbl = $(parameters_tbl);

    const resp = await $.getJSON(`/parameters?device=${device}`);

    if (Object.keys(resp).length === 0) {
        $parameters_tbl.html(`
            <tr>
                <td colspan="2" style="text-align: center">
                    <i>No parameters can be configured.</i>
                </td>
            </tr>
        `);
    } else {
        $parameters_tbl.html(Object.entries(resp).map((entry) => {
            const [name, value] = entry;
            return `
                <tr>
                    <td style="vertical-align: middle">${name}</td>
                    <td>
                        <input type="number" class="form-control ${parameter_input}" data-device="${device}" data-parameter="${name}" value="${parseFloat(value)}" />
                    </td>
                </tr>    
            `;
        }));
    }
}

function parseTimestamp(dateline) {
    const date = new Date(parseInt(dateline));
    return date.toLocaleDateString() + ", " + date.toLocaleTimeString();
}

window.iot = {
    init: async () => {
        $(document).on('change', device_select, async (e) => {
            await loadDevice($(`${device_select} option:selected`).attr('data-address'));
        });

        $(document).on('change', `.${parameter_input}`, async function (e) {
            const $this = $(this);
            const device = $this.attr('data-device');
            const parameter = $this.attr('data-parameter');
            const value = $this.val();

            $this.removeClass('is-valid').removeClass('is-invalid');

            const res = await changeParameter(device, parameter, value);

            if (res) {
                $this.addClass('is-valid');
            } else {
                $this.addClass('is-invalid');
            }
        });

        await loadDevices();
        await loadDevice($(`${device_select} option:selected`).attr('data-address'));
    }
};
