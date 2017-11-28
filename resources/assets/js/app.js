
/**
 * First we will load all of this project's JavaScript dependencies which
 * includes Vue and other libraries. It is a great starting point when
 * building robust, powerful web applications using Vue and Laravel.
 */

require('./bootstrap');

// let socket = io();

let socket = io.connect(`${window.location.hostname}:3000`);

// emit event authenticate when connect
socket.on('connect', function () {
    console.log('Connected to SocketIO, Authenticating')
    socket.emit('authenticate', {token: jwtToken});
});

// if authenticated
socket.on('authenticated', function () {
    console.log('Authenticated');
});

// if unauthorized
socket.on('unauthorized', function (data) {
    console.log('Unauthorized, error: ' + data.message);
});

// if disconnect
socket.on('disconnect', function () {
    console.log('Disconnected');
});

// if click button
$('#button').change(function (event) {
    var data;
    if ($(this).prop('checked')) {
        data = 'on';
    } else {
        data = 'off';
    }

    console.log('Change led state:' + data);
    // emit event led-change
    socket.emit('led-change', data);
});
