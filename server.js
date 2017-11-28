require('dotenv').config();
var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var colors = require('colors');
var port = process.env.SOCKETIO_PORT || 3000;
var socketioJwt = require('socketio-jwt');

server.listen(port, function () {
    console.log('Server listening at port %d', port);
});

// Let express show index.html to client
app.use(express.static(__dirname + '/static'));

// Accept connection and authorize token
io.on('connection', socketioJwt.authorize({
    secret: process.env.JWT_SECRET,
    timeout: 15000
}));

// When authenticated, send back name + email over socket
io.on('authenticated', function (socket) {
    getRole(socket);
    socket.emit('info', socket.decoded_token);
});

function getRole(socket) {
    switch (socket.decoded_token.guard.toLowerCase()) {
        case 'device':
            addDevice(socket);
            break;
        case 'user':
            addUser(socket);
            break;
    }
}

// Handling each role
function addDevice(socket) {
    console.log(('New device ' + socket.id + ' connected!').green);
    console.log(socket.decoded_token);

    socket.join('devices_room');
    socket.on('device_state', function (data) {
        io.sockets.in('users_room').emit('device_state', data);
    });
    socket.on('disconnect', function () {
        console.log(('Device ' + socket.id + ' disconnected').gray);
    });
}

function addUser(socket) {
    console.log(('New user ' + socket.id + ' connected!').green);
    console.log(socket.decoded_token);
    socket.join('users_room');
    socket.on('led-change', function (data) {
        io.sockets.in('devices_room').emit('led-change', data);
        console.log('Change device state: '.yellow, (data).blue);
    });
    socket.on('disconnect', function () {
        console.log(('User ' + socket.id + ' disconnected').gray);
    });
}
