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

//DCC signal defines
var timer = new NanoTimer();
const hBridgeA = new Gpio(17, 'out');
const hBridgeB = new Gpio(18, 'out');
const hBridgeC = new Gpio(19, "out");
const hBridgeD = new Gpio(19, "out");
