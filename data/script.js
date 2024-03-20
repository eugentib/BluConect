var socket;

function init() {
    socket = new WebSocket('ws://' + window.location.hostname + '/WS');
    
    socket.onopen = function(event) {
        console.log("Conexiune deschisă");
    };

    socket.onmessage = function(event) {
        document.getElementById('output').textContent += event.data + '\n';
    };

    socket.onerror = function(error) {
        console.log('Eroare WebSocket:', error);
    };
}

function sendSerialData() {
    var data = document.getElementById('inputData').value;
    socket.send(data);
}

function setBaudRate() {
    var baudRate = document.getElementById('baudRate').value;
    socket.send("SET_BAUD_RATE " + baudRate);
}

window.onload = init;
