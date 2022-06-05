/*

██████╗  ██████╗ ██████╗██╗  ██╗ ██████╗ ██╗  ██╗
██╔══██╗██╔════╝██╔════╝██║  ██║██╔════╝ ██║  ██║
██║  ██║██║     ██║     ███████║███████╗ ███████║
██║  ██║██║     ██║     ██╔══██║██╔═══██╗╚════██║
██████╔╝╚██████╗╚██████╗██║  ██║╚██████╔╝     ██║
╚═════╝  ╚═════╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝      ╚═╝
A digital scale railroad controller by erikd256.


Licensed under the GNU GPL 3.0 or later
*/

//DCC signal required modules
const NanoTimer = require('nanotimer');
const Gpio = require('onoff').Gpio;
const { Server } = require("socket.io");
const figlet = require("figlet");

//DCC signal defines
var timer = new NanoTimer();
const io = new Server();
const hBridgeA = new Gpio(17, 'out');
const hBridgeB = new Gpio(18, 'out');
const hBridgeC = new Gpio(19, "out");
const hBridgeD = new Gpio(19, "out");

// create some beautiful figlet text

console.log(figlet.textSync('DCCH64 MainStation', {
  font: 'ANSI Regular',
  horizontalLayout: 'default',
  verticalLayout: 'default',
  width: 80,
  whitespaceBreak: false
}));

// start socket.io server at port 443964 (acronym for DCCH64 btw)

io.on("connection", (socket) => {
  console.log("client connected | client id: "+socket.id);
  socket.on("disconnecting", (reason) => {
    console.log("client disconnected |" + reason + " | client id: "+socket.id);
  });
  socket.on("rawPacket", (data) => {
    console.log(data);
  });
});

io.listen(443964);

// Unexport pins on process end to prevent damage and free up resources.
process.on('SIGINT', _ => {
    hBridgeA.unexport();
    hBridgeB.unexport();
    hBridgeC.unexport();
    hBridgeD.unexport();
});
