const path = require('path');

module.exports = {
    entry: path.resolve(__dirname, 'src/public/assets/scripts/src'),
    mode: "development",
    module: {
        rules: [{
            exclude: /node_modules/,
            test: /\.js$/,
            use: ['babel-loader']
        }]
    },
    output: {
        filename: 'combined.min.js',
        path: path.resolve(__dirname, 'src/public/assets/scripts')
    }
};