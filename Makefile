#Makefile for DCCH64 MainStation
all:
<<<<<<< HEAD
	g++ -O3 -Wall main.cpp -o mainstation-cli -lwiringPi -lpthread
install:
	cp mainstation-cli /usr/local/bin/
	cp dcc.h /usr/local/bin
=======
	g++ -O3 -Wall main.cpp -o mainstation-interactive -lwiringPi -lpthread
>>>>>>> master
