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
	string message = "";

	message.append(&ioStart);
	message.append(String(type).c_str());
	message.append(&ioSeperator);
	message.append(String(xPos).c_str());
	message.append(&ioSeperator);
	message.append(String(yPos).c_str());
	message.append(&ioSeperator);
	message.append(String(millis()).c_str());
	message.append(&ioEnd);

	sendBuffer.enqueue(message);

	//sendBuffer.enqueue(
	//	ioStart + String(type).c_str() + 
	//	ioSeperator + String(xPos).c_str() +
	//	ioSeperator + String(yPos).c_str() +
	//	ioSeperator + String(millis()).c_str() +
	//	ioEnd);
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

BluetoothController::rxPackage BluetoothController::unpackMessage(string message)
{
	rxPackage package;
	vector<string> subMessage;

	//removing start and end notation
	int startIndex = -1;
	int endIndex = -1;
	for (int i = 0; i < message.length(); i++)
	{
		char c = message.at(i);
		if (c == ioStart)
			startIndex = i;
		else if (c == endIndex)
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

	message = (message.substr(startIndex + 1, endIndex - 1));

	//if (message.startsWith(String(ioStart)) || message.endsWith(String(ioEnd)))
	//{
	//	Serial.println("BTC: incorrect message format, missing start and/or end notaion");
	//	return;
	//}
	//message = (message.substring(1, message.length() - 2));

	//seperate by io seperator
	string sub = ""; 
	for (int i = 0; i < message.length(); i++)
	{
		if (message.at(i) == ioSeperator)
		{
			subMessage.push_back(sub);
			sub = "";
			continue;
		}
		sub += message.at(i);
	}
	subMessage.push_back(sub);

	//add to package
	//	type decoder
	if (subMessage.at(0).length() != 1)
	{
		Serial.print("BTC: wrong type format, ");
		//Serial.println(subMessage.at(0).);
	}
	package.type = (reciveType_e)subMessage.at(0).at(0);

	//	sepperating data
	String data = "";
	for (int i = 1; i < subMessage.at(1).length(); i++)
	{ 
		char c = subMessage.at(i).at(i);
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

