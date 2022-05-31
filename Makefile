#Makefile for DCCH64 MainStation
all:
	mkdir out
	g++ -O3 -Wall main.cpp -o out/mainstation-cli -lwiringPi -lpthread
install:
	cp mainstation-cli /usr/local/bin/
	cp dcc.h /usr/local/bin
