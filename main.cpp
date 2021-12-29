#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <string>
typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
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
char* chars_array;
typedef server::message_ptr message_ptr;
void runPacket() {
	//build the packet
	while(session) {
		mu.lock();
		buildPacket(_dir, _address, _speed);
		mu.unlock();
		sendPacket();
	}
}
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	//create and run the thread
	cout << msg->get_payload();
	thread t(&runPacket);
	// Set the speed setting to 100 to quit
	string output;
	output = string(msg->get_payload());
  string str_obj(output);
	chars_array = strtok(&str_obj[0], "|");
	cmd = chars_array[1];
	Address = chars_array[2];
		mu.lock();
		if(cmd > 0 && cmd < 128) {
			_dir = 1;
			_speed = cmd;
      _address = Address;
		}
		else if(cmd < 0 && cmd > -128) {
			_dir = 0;
			_speed = -cmd;
      _address = Address;
		}
		else {
			_dir = 1;
			_speed = 0;
      _address = Address;
		}
		mu.unlock();

    if (msg->get_payload() == "exit") {
        s->stop_listening();
				dccClose();
        return;
    }
}

int main() {
    server dcch64_websocket;
    try {
        dcch64_websocket.set_access_channels(websocketpp::log::alevel::all);
        dcch64_websocket.clear_access_channels(websocketpp::log::alevel::frame_payload);
        dcch64_websocket.init_asio();
        dcch64_websocket.set_message_handler(bind(&on_message,&dcch64_websocket,::_1,::_2));
        dcch64_websocket.listen(4338);
				cout << "----------------------" << endl;
				cout << "| DCCH64 MainStation |" << endl;
				cout << "----------------------" << endl;
				cout << "Now listening at port 4338" << endl;
				//setup dcc
				dccSetup();
        // Start the server accept loop
        dcch64_websocket.start_accept();
        // Start the ASIO io_service run loop
        dcch64_websocket.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
