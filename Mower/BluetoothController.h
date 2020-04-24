#pragma once

#include "Queue.h"
#include <ArduinoSTL.h>
#include <stdlib.h>


using namespace std;
class BluetoothController
{
public:

	enum EventType_e
	{
		literallyAnythingElse = '0',
		crachAvoidance ='1',
	};
	
	enum reciveType_e
	{
		turnOff = 'Q',

		drive ='D',
		manuall = 'M',
		atomatic = 'A',
		ligh = 'L',
		honk = 'H',
	};

	BluetoothController();
	~BluetoothController();

	void run();

	void send(EventType_e,int,int);
	void addReciveListner(reciveType_e,void(*callback)(int[],int));

private:

	const char ioStart = '@';
	const char ioEnd = '$';
	const char ioSeperator = ',';
	const char ioDataSeperator = ';';

	struct rxListner_s
	{
		reciveType_e type;
		void(*callback)(int[], int);
	};
	struct rxPackage
	{
		reciveType_e type;
		vector<int> data;
	};

	Queue<String> sendBuffer;
	vector<rxListner_s> listners;


	void reciveListnerBT(String);
	rxPackage unpackMessage(String);
	void sendToListner(rxPackage);

	Queue<String> rxBuffer;


};

