ws = require("ws");
const wss = new ws.WebSocketServer({ port: 3000 });
var cp = require('child_process');
var mainstationCLI = cp.spawn('mainstation-cli');
var shell = require('shelljs');

mainstationCLI.stdin.write("0" + "\n");
mainstationCLI.stdin.write("0" + "\n");

function sendToInteractive(msg, ws){

  if(msg == "stop"){
    mainstationCLI.stdin.write('0' + "\n");
    mainstationCLI.stdin.write("0" + "\n");
  }else if(msg == "poweroff"){
    mainstationCLI.stdin.write('100' + "\n");
    mainstationCLI.stdin.write('100' + "\n");
    shell.exec('sudo poweroff')
  }
  else{
    const cmd = msg;
  const cmdArray = cmd.toString().split("|");
  console.log("Speed setting: " + cmdArray[0]);
  console.log("Address setting: " + cmdArray[1]);
  mainstationCLI.stdin.write(cmdArray[0]+ "\n");
  mainstationCLI.stdin.write(cmdArray[1]+ "\n");
  }

};

wss.on('connection', function connection(ws) {
  ws.on('message', function message(data) {
    console.log('received: %s', data);
    sendToInteractive(data, ws);
  });
  ws.send('server accepted connection');
});
wss.on("close", function close(ws){
  mainstationCLI.stdin.write('100' + "\n");
  mainstationCLI.stdin.write('100' + "\n");
})
