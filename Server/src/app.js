import './sass/app.scss';
require('./bootstrap');

let socket = io();

$('#button').change(function (event) {
    var data;
    if ($(this).prop('checked')) {
        data = 'on';
    } else {
        data = 'off';
    }

    socket.emit('led-change', data);
});
