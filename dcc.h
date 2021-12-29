/*

A simple set of functions to get up and running with DCC using the Raspberry pi.

To use the library you will need wiringPi installed on the pi!
sudo apt-get install wiringPi -y

after that you will first need to run dccSetup(int pin1, int pin2) replacing pins
with the 2 pins used in your circuit. The default pins are 0 and 2 with the wiringPi
pin numbers.

To see the wiringPi pin numbers then run the following command
gpio readall

Once setup has run you build the packet to send by the function
buildPacket(int dir, int address, int speed)
dir is direction with 1 meaning forward and 0 backwards
address is the train address i.e 3 for address 00000011

then speed which is between 0 and 27 max
0 is full stop and between 0 and 27 are increments of speed

*/

//Check if library is already included?
#ifndef __DCC_LIBRARY__
#define __DCC_LIBRARY__

#include <wiringPi.h>

//wiring pi pin numbers
int pinA = 2;
int pinB = 0;

const char preamble[] = {1,1,1,1, 1,1,1,1, 1,1};
char address[8];
char packet[] = {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1};
const char steps[][5] = {{0,0,0,0,0},{0,0,0,1,0},{1,0,0,1,0},{0,0,0,1,1},{1,0,0,1,1},{0,0,1,0,0},{1,0,1,0,0},{0,0,1,0,1},{1,0,1,0,1},{0,0,1,1,0},{1,0,1,1,0},{0,0,1,1,1},{0,1,0,0,0},{1,1,0,0,0},{0,1,0,0,1},{1,1,0,0,1},{0,1,0,1,0},{1,1,0,1,0},{0,1,0,1,1},{1,1,0,1,1},{0,1,1,0,0},{1,1,1,0,0},{0,1,1,0,1},{1,1,1,0,1},{0,1,1,1,0},{1,1,1,1,0},{0,1,1,1,1},{1,1,1,1,1}};


//Speed max number = 27
//address is train number in int form
//direction os 1 for forward and 0 for backwards
void buildPacket(int dir, int address, int speed) {

	//error checking input variables
	if(speed > 27) {
		speed = 27;
	}
	if(speed < 0) {
		speed = 0;
	}

	if(dir > 1) {
		dir = 1;
	}
	if(dir < 0) {
		dir = 0;
	}

	//okay lets build a packet with the data input
	int pos = 0;

	//preamble first
	for(int i = 0; i < (int)(sizeof(preamble) / sizeof(char)); i++) {
		packet[i] = preamble[i];
		pos ++;
	}

	//add the end bit
	packet[pos] = 0;
	pos ++;

	//next lets add the address to the packet
	//but first we need to turn it into 1's and 0's
	int aBit = pos;
	char a[8];

	a[0] = (address & 0b10000000) >> 7;
	a[1] = (address & 0b01000000) >> 6;
	a[2] = (address & 0b00100000) >> 5;
	a[3] = (address & 0b00010000) >> 4;
	a[4] = (address & 0b00001000) >> 3;
	a[5] = (address & 0b00000100) >> 2;
	a[6] = (address & 0b00000010) >> 1;
	a[7] = (address & 0b00000001);

	for(int i = 0; i < 8; i++) {
		packet[pos] = a[i];
		pos ++;
	}

	//add the end bit
	packet[pos] = 0;
	pos++;

	int dBit = pos;

	//now for the direction and speed
	packet[pos] = 0;
	pos++;
	packet[pos] = 1;
	pos++;
	packet[pos] = dir;
	pos++;

	for(int i = 0; i < 5; i++) {
		packet[pos] = steps[speed][i];
		pos++;
	}

	//add the end bit
	packet[pos] = 0;
	pos++;

	//error checking
	for(int i = 0; i < 8; i++) {
		if(packet[aBit+i] == packet[dBit+i]) {
			packet[pos] = 0;
			pos++;
		}
		else {
			packet[pos] = 1;
			pos++;
		}
	}

	//add the end bit
	packet[pos] = 1;
}

//Send the packet to the train using the desired two pins for the H-Bridge circuit
void sendPacket() {

	//get the packet size so we know how many bits to send
	int length = sizeof(packet) / sizeof(char);

	//loop through sending bits
	for(int i = 0; i < length; i++) {

		//always turn a pin off before turning on!!!
		//check for 1 or 0?
		if(packet[i] == 1) {
			digitalWrite(pinA, LOW);
			digitalWrite(pinB, HIGH);
			delayMicroseconds(58);
			digitalWrite(pinB, LOW);
			digitalWrite(pinA, HIGH);
			delayMicroseconds(58);
		}
		else {
			digitalWrite(pinA, LOW);
			digitalWrite(pinB, HIGH);
			delayMicroseconds(100);
			digitalWrite(pinB, LOW);
			digitalWrite(pinA, HIGH);
			delayMicroseconds(100);
		}
	}
}

//just turns off both pins so no power is going through the track
void dccClose() {
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
}

//Simple function which must be called first to start the library and set up the pins and wiring pi
//p1 and the p2 are the pin numbers used as found in the wiring pi library. To check this simply
//type gpio readall and see which pins you want to use. The dault is pin0 and pin2. the order
//doesn't matter
void dccSetup(int p1 = 0, int p2 = 2) {

	//Setup the pins
	pinA = p1;
	pinB = p2;

	//Start library
	wiringPiSetup();

	//setup the pins
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
}

#endif
