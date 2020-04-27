#pragma once

#include "Queue.h"
#include <ArduinoSTL.h>
#include <stdlib.h>

using namespace std;
class BluetoothController
{
public:
	
	//types of events to send to app
	enum EventType_e
	{
		literallyAnythingElse = '0',
		crachAvoidance ='1',
	};
	
	//types of events recivable from the app
	enum reciveType_e
	{
		turnOff = 'Q',

		drive ='D',
		manuall = 'M',
		atomatic = 'A',
		ligh = 'L',
		honk = 'H',
	};
	static BluetoothController* getInstance()
	{
		static BluetoothController* instance = nullptr;
		if (instance == nullptr)
			instance = new BluetoothController();
		return instance;
	}
	~BluetoothController();

	//the main state machine 
	void run();

	//initialize the class, NOTE must be called once before usage
	void init(int baud);

	//send an event to the app togehter with XY coordinates
	void send(EventType_e,int,int);
	//adds a listner that listen for a specific rx type and will be called whe it is recived
	void addReciveListner(reciveType_e,void(*callback)(int[],int));

private:
	BluetoothController();

	struct rxListner_s
	{
		reciveType_e type;
		void(*callback)(int[], int);
	};
	struct rxPackage
	{
		reciveType_e type;
		bool acknowledge;
		vector<int> data;
	};

	//function to be called when 
	static void reciveListnerBT(String message)
	{
		getInstance()->rxBuffer.enqueue(message);
	}
	rxPackage unpackMessage(String);
	bool msgIsAck(String);
	String trim(String);
	String* split(String);
	rxPackage map(String, String, String);
	void sendAcknowledge(reciveType_e);

	void sendToListner(rxPackage);

	void runTX();
	void runRX();



	//message format charachters
	const char ioStart = '@';
	const char ioEnd = '$';
	const char ioSeperator = ',';
	const char ioDataSeperator = ';';
	
#define RX_PACKAGE_SIZE 3

	bool ackRecived = false;

	//buffer containing messages to be sent, in order
	Queue<String> sendBuffer;
	vector<rxListner_s> listners;
	Queue<String> rxBuffer;


};
