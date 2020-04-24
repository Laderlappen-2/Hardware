#include "BluetoothController.h"
#include <Arduino.h>


BluetoothController::BluetoothController()
{
	btTranciver.setListener(this->reciveListnerBT);
}


BluetoothController::~BluetoothController()
{
}

void BluetoothController::run()
{
	btTranciver.run();
	runTX();
	runRX();
}
void BluetoothController::runTX()
{
	if (sendBuffer.isEmpty())
		return;
	btTranciver.sendDataExpectAck(sendBuffer.dequeue());
}

void BluetoothController::runRX()
{
	if (rxBuffer.isEmpty())
		return;
	sendToListner(unpackMessage(rxBuffer.dequeue()));
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



//void BluetoothController::reciveListnerBT(String message)
//{
//	getInstance()->rxBuffer.enqueue(message);
//}

BluetoothController::rxPackage BluetoothController::unpackMessage(String message)
{
	rxPackage package;
	vector<String> subMessage;

	//removing start and end notation
	if (message.startsWith(String(ioStart)) || message.endsWith(String(ioEnd)))
	{
		Serial.println("BTC: incorrect message format, missing start and/or end notaion");
		return;
	}
	message = (message.substring(1, message.length() - 2));

	//seperate by io seperator
	String sub = ""; 
	for (int i = 0; i < message.length(); i++)
	{
		if (message.charAt(i) == ioSeperator)
		{
			subMessage.push_back(sub);
			sub = "";
			continue;
		}
		sub += message.charAt(i);
	}
	subMessage.push_back(sub);

	//add to package
	//	type decoder
	if (subMessage.at(0).length() != 1)
	{
		Serial.print("BTC: wrong type format, ");
		Serial.println(subMessage.at(0));
	}
	package.type = (reciveType_e)subMessage.at(0).charAt(0);

	//	sepperating data
	String data = "";
	for (int i = 1; i < subMessage.at(1).length(); i++)
	{ 
		char c = subMessage.at(i).charAt(i);
		if (c == ioDataSeperator)
		{
			package.data.push_back(data.toInt());
			data = "";
			continue;
		}
		data += c;
	}
	package.data.push_back(data.toInt());

	return package;
}
