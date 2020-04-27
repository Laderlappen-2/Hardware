#include "BluetoothController.h"
#include <Arduino.h>


BluetoothController::BluetoothController()
{
}

void BluetoothController::init(int baud)
{
	Serial.begin(baud);
}

BluetoothController::~BluetoothController()
{
}

void BluetoothController::run()
{
	runTX();
	runRX();
}
void BluetoothController::runTX()
{
	enum state_e
	{
		idle,
		sending,
		waiting,
	};
	static state_e state = idle;
	static const int waitTimeout_ms = 3000;
	static long startWait = 0;
	static String message = "";

	switch (state)
	{
	case idle:
		if (!sendBuffer.isEmpty())
			state = sending;
		message = sendBuffer.dequeue();

		break;
	case sending:
		Serial.print(message);
		startWait = millis();
		state = waiting;

		break;
	case waiting:

		if (ackRecived)
		{
			state = idle;
			ackRecived = false;
		}

		if (millis() > startWait + waitTimeout_ms)
			state = sending;
		break;
	default:
		break;
	}
}

void BluetoothController::runRX()
{
	if (Serial.available())
		rxBuffer.enqueue(Serial.readString());

	if (rxBuffer.isEmpty())
		return;

	String message = rxBuffer.dequeue();
	if (msgIsAck(message))
	{
		ackRecived = true;
		return;
	}

	rxPackage package = unpackMessage(message);
	sendToListner(package);
	
	if (package.acknowledge)
		sendAcknowledge(package.type);
}

void BluetoothController::send(EventType_e type, int xPos, int yPos)
{
	sendBuffer.enqueue(
		ioStart + String(type) + 
		ioSeperator + String(xPos) + 
		ioSeperator + String(yPos) +
		ioSeperator + String(millis()) +
		ioEnd);
}

void BluetoothController::addReciveListner(reciveType_e type, void(*callback)(int[], int))
{
	if (callback == nullptr)
	{
		Serial.println("BTC: can't set callback, function is nullptr");
		return;
	}
	listners.push_back(rxListner_s{ type,callback });
}

void BluetoothController::sendToListner(rxPackage package)
{
	for (int i = 0; i < listners.size(); i++)
	{
		if (listners.at(i).type == package.type)
		{
			int* data = new int[package.data.size()];
			for (int j = 0; j < package.data.size(); j++)
			{
				data[j] = package.data.at(j);					
			}
			listners.at(i).callback(data, package.data.size());
			delete data;
		}
	}
}



BluetoothController::rxPackage BluetoothController::unpackMessage(String rawMessage)
{
	String message = trim(rawMessage);
	String * subMessage = split(message);
	rxPackage package = map(subMessage[0], subMessage[1], subMessage[2]);
	delete subMessage;
	return package;
}

bool BluetoothController::msgIsAck(String message)
{
	String str = trim(message);
	return str.endsWith("A");
}

String BluetoothController::trim(String message)
{
	//removing start and end notation
	int startIndex = -1;
	int endIndex = -1;
	for (int i = 0; i < message.length(); i++)
	{
		char c = message.charAt(i);
		if (c == ioStart)
			startIndex = i;
		else if (c == ioEnd)
		{
			endIndex = i;
			break;
		}
	}
	if (startIndex < 0)
		Serial.println("BTC: missing start notation");
	if (endIndex < 0)
		Serial.println("BTC: missing start notation");
	if (startIndex < 0 || endIndex < 0)
		return;

	return (message.substring(startIndex + 1, endIndex));
}
String * BluetoothController::split(String message)
{	
	//seperate by io seperator
	String *subMessage = new String[RX_PACKAGE_SIZE];	
	String str = "";
	int index = 0;
	for (int i = 0; i < message.length(); i++)
	{
		if (message.charAt(i) == ioSeperator)
		{
			subMessage[index] = str;
			str = "";
			continue;
		}
		str += message.charAt(i);
	}
	return subMessage;
}
BluetoothController::rxPackage BluetoothController::map(String type, String data, String ack)
{
	rxPackage package;


	String value = "";
	for (int i = 1; i < data.length(); i++)
	{
		char c = data.charAt(i);
		if (c == ioDataSeperator)
		{
			package.data.push_back(value.toInt());
			value = "";
			continue;
		}
		value += c;
	}
	package.data.push_back(value.toInt());

	package.type = (reciveType_e)type.charAt(0);
	package.acknowledge = ack.charAt(0) == '1';

	return package;
}

void BluetoothController::sendAcknowledge(reciveType_e type)
{
	String ackMsg =
		ioStart + String(type) +
		ioSeperator + 'A' +
		ioEnd;
	Serial.print(ackMsg);
}
