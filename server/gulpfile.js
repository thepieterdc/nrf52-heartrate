"use strict";

const gulp = require('gulp');
const concat = require('gulp-concat');
const minify = require('gulp-clean-css');
const webpack = require('webpack-stream');

const css = ['node_modules/bootswatch/dist/litera/bootstrap.min.css'];
const js = ['src/public/assets/scripts/src/*.js'];
const vendorJs = ['node_modules/jquery/dist/jquery.min.js', 'node_modules/moment/min/moment.min.js', 'node_modules/chart.js/dist/Chart.js'];

gulp.task('css', () => {
    return gulp.src(css)
        .pipe(concat('combined.min.css'))
        .pipe(minify())
        .pipe(gulp.dest('src/public/assets/stylesheets/'));
});

gulp.task('js', () => {
    return gulp.src('src/public/assets/scripts/')
        .pipe(webpack(require('./webpack.config')))
        .pipe(gulp.dest('src/public/assets/scripts/'));
});

gulp.task('vendor-js', () => {
    return gulp.src(vendorJs, {nosort: true})
        .pipe(concat('vendor.min.js'))
        .pipe(gulp.dest('src/public/assets/scripts/'));
});

gulp.task('watch', () => {
    gulp.watch(js, gulp.series('js'));
});

gulp.task('default', gulp.parallel('css', 'js', 'vendor-js', (done) => done()));