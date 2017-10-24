var SerialPort = require('serialport');
var serialPort = new SerialPort('/dev/cu.usbmodem1411', {
  baudRate: 9600
});

// var app = require('express')();
const express = require('express');
const winning_score = 3;
const app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var port = process.env.PORT || 3000;
var soundPlayer = require('play-sound')(opts = {})

var player1 = 0;
var player2 = 0;

app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

app.use(express.static('public'));

io.on('connection', function(socket){
	serialPort.on('data', function (data) {
  	var winner = data.toString('utf8');
    console.log("Data IS: ");
    console.log(data);
  	if(winner == 1) {
  		player1++;
  		socket.emit("player1", player1);
      if(player1 == winning_score) {
        soundPlayer.play('player1_wins.mp3', function(err){ /* Do nothing if err */ })
        socket.emit("playerOneWin");
        serialPort.write('f', function(err) {
          if (err) {
            return console.log('Error on write: ', err.message);
          }
          console.log('game finished');
        });
      }
  	} else if(winner == 2) {
  		player2++;
  		socket.emit("player2", player2);
      if(player2 == winning_score) {
        soundPlayer.play('player2_wins.mp3', function(err){ /* Do nothing if err */ })
        socket.emit("playerTwoWin");

        serialPort.write('f', function(err) {
          if (err) {
            return console.log('Error on write: ', err.message);
          }
          console.log('game finished');
        });
      }
  	}
	});

  socket.on('reset', function(msg){
  	player1 = 0;
  	player2 = 0;
  });

  socket.on('start', function(msg) {
    serialPort.write('s', function(err) {
      if (err) {
        return console.log('Error on write: ', err.message);
      }
      console.log('game started');
    });
  });
});

http.listen(port, function(){
  console.log('listening on *:' + port);
});
