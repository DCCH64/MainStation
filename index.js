/*
██████╗  ██████╗ ██████╗██╗  ██╗ ██████╗ ██╗  ██╗
██╔══██╗██╔════╝██╔════╝██║  ██║██╔════╝ ██║  ██║
██║  ██║██║     ██║     ███████║███████╗ ███████║
██║  ██║██║     ██║     ██╔══██║██╔═══██╗╚════██║
██████╔╝╚██████╗╚██████╗██║  ██║╚██████╔╝     ██║
╚═════╝  ╚═════╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝      ╚═╝
A digital scale railroad controller by erikd256. - Licensed under the GNU GPL 3.0 or later
*/

//DCC signal required modules
const NanoTimer = require('nanotimer');
const Gpio = require('onoff').Gpio;
const { Server } = require("socket.io");
const figlet = require("figlet");
const binaryConverter = require("int-to-binary");
// WebServer and NanoTimer required for package processing
var timer = new NanoTimer();
const io = new Server();

// A and B are for the track output, C and D are for the optional status light.
const hBridgeA = new Gpio(17, 'out');
const hBridgeB = new Gpio(27, 'out');
const hBridgeC = new Gpio(5, "out");
const hBridgeD = new Gpio(6, "out");

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
  // enable status light
  hBridgeC.writeSync(1);
  hBridgeD.writeSync(1);
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
    hBridgeA.writeSync(0);
    hBridgeB.writeSync(0);
    hBridgeC.writeSync(0);
    hBridgeD.writeSync(0);
    hBridgeA.unexport();
    hBridgeB.unexport();
    hBridgeC.unexport();
    hBridgeD.unexport();
});

/*

DCC Packet:

General format:

14 bits with the value 1 as preamble byte + 0 + Address byte + 0 + instruction byte + 0 + error detection byte + 1

*/

// define empty packet we're going to fill up later
var packet = [];

// define speedsteps 
var speedSteps = [10000,00001,10001,00000,,00010,10010,00011,10011,00100,10100,00101,10101,00110,10110,00111,10111,01000,11000,01001,11001,01010,11010,01011,11011,01100,11100,01101,11101,01110,11110,01111,11111];

// calculate address byte as it consist of an 8-bit binary string.
function calculateAddressB(addr) {
  var address = binaryConverter.unsigned(addr, 8);
  var addressbit = address.split("");
  return addressbit;
}

function calculateInstructionB(fxGroup, bit1, bit2, bit3, bit4, bit5){
  /* 
  instruction byte = CCCDDDDD 0 DDDDDDDD 0 DDDDDDD 0
  
  "1" means fx is on, "0" means fx is off

  Function group one instructions (F0-F4): 100DDDDD
    - First D-bit 
  
  Function group two instructions (F5-F12)
  */
  var instructionbit = [1,0,0,D,D,D,D,D];
  if(fxGroup == 1){
    instructionbit[2] = 0;
    if(bit1 == "1"){
      instructionbit[3] = 1;
    }else{
      instructionbit[3] = 0;
    };
    if(bit2 == "1"){
      instructionbit[4] = 1;
    }else{
      instructionbit[4] = 0;
    };
    if(bit3 == "1"){
      instructionbit[5] = 1;
    }else{
      instructionbit[5] = 0;
    };
    if(bit4 == "1"){
      instructionbit[6] = 1;
    }else{
      instructionbit[6] = 0;
    };
    if(bit5 == "1"){
      instructionbit[7] = 1;
    }else{
      instructionbit[7] = 0;
    };
  };
  if(fxGroup == 1){
    instructionbit[2] = 1;
    if(bit1 == "1"){
      instructionbit[3] = 1;
    }else{
      instructionbit[3] = 0;
    };
    if(bit2 == "1"){
      instructionbit[4] = 1;
    }else{
      instructionbit[4] = 0;
    };
    if(bit3 == "1"){
      instructionbit[5] = 1;
    }else{
      instructionbit[5] = 0;
    };
    if(bit4 == "1"){
      instructionbit[6] = 1;
    }else{
      instructionbit[6] = 0;
    };
    if(bit5 == "1"){
      instructionbit[7] = 1;
    }else{
      instructionbit[7] = 0;
    };
  };
  return instructionbit;  
}


function createPacket(address, speed){
  var speedStepPosition = speed + 3;
  console.log(speedSteps[speedStepPosition].split());
}


