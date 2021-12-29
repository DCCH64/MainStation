#include <iostream>
#include <thread>
#include <mutex>
#include "dcc.h"

using namespace std;

mutex mu;
int cmd = 0;
int _dir = 1;
int _speed = 0;
int _address = 3;
bool session = true;
int Address = 0;


void runPacket() {
	//build the packet
	while(session) {
		mu.lock();
		buildPacket(_dir, _address, _speed);
		mu.unlock();
		sendPacket();
	}
}

int main(int argc, char * argv[]) {

	cout << "Steve's Dcc Train Commander" << endl;
	cout << "------------------------------------" << endl;

	//setup dcc
	dccSetup();

	//create and run the thread
	thread t(&runPacket);

	//Simply type 100 to quit
	//lazy but easy
	while(cmd != 100) {

		cout << "Speed setting : " << flush;
		cin >> cmd;
		cout << "Address setting : " << flush;
		cin >> Address;

		mu.lock();
		if(cmd > 0 && cmd < 28) {
			_dir = 1;
			_speed = cmd;
			_address = Address;
		}
		else if(cmd < 0 && cmd > -28) {
			_dir = 0;
			_speed = -cmd;
			_address = Address;
		}
		else {
			_dir = 1;
			_speed = 0;
		}
		mu.unlock();
	}

	//close the thread and get it to rejoin!
	session = false;
	t.join();

	//cleanup
	dccClose();

	return 0;
}
