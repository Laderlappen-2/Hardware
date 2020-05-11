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
    {
			state = sending;
		  message = sendBuffer.dequeue();
    }

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
  String message = "";
  while(Serial.available() > 0)
  {
    delay(10);
    char currentChar = Serial.read();
    message += currentChar;

    if(currentChar == '$')
    {
      Serial.println("MESSAGE DONE READING: " + message);
      //rxBuffer.enqueue(message);
      //break;
    }
  }
  
  if(message == "")
    return;

  Serial.println("Hello World!");
  
	if (msgIsAck(message))
	{
		ackRecived = true;
		return;
	}

	//Serial.println("IN runRX(): " + message);
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
	Serial.println("IN sendToListner()");
	for (int i = 0; i < listners.size(); i++)
	{
    Serial.println(listners.at(i).type);
    Serial.println(package.type);
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
 Serial.println("LEAVING sendToListner()");
}



BluetoothController::rxPackage BluetoothController::unpackMessage(String rawMessage)
{
	String message = trim(rawMessage);
  Serial.println("TRIM DONE");
	vector<String> subMessage = split(message, ioSeperator);
  Serial.println("SPLIT DONE");
	rxPackage package = map(subMessage.at(0), subMessage.at(1), subMessage.at(2));
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
	Serial.println(message);
	/*int startIndex = -1;
	int endIndex = -1;
	for (int i = 0; i < message.length(); i++)
	{
		char c = message.charAt(i);
		if (c == ioStart)
		{
			startIndex = i;
			Serial.println(i);
		}
		else if (c == ioEnd)
		{
			endIndex = i;
			Serial.println(i);
			break;
		}
	}
	if (startIndex < 0)
		Serial.println("BTC: missing start notation");
	if (endIndex < 0)
		Serial.println("BTC: missing start notation");
	if (startIndex < 0 || endIndex < 0)
		return;*/

  int startIndex = message.indexOf(ioStart);
  int endIndex = message.indexOf(ioEnd);

	return (message.substring(startIndex + 1, endIndex));
}
vector<String> BluetoothController::split(String message, char separator)
{	
	//seperate by io seperator
	/*String *subMessage = new String[RX_PACKAGE_SIZE];	
	String str = "";
	int index = 0;
	for (int i = 0; i < message.length(); i++)
	{
		if (message.charAt(i) == ioSeperator)
		{
			subMessage[index] = str;
      index++;
			str = "";
			continue;
		}
		str += message.charAt(i);
	}
  subMessage[RX_PACKAGE_SIZE - 1] = str;
  Serial.print(subMessage[0]);
  Serial.print(subMessage[1]);
  Serial.println(subMessage[2]);
	return subMessage;*/

  vector<String> returnVector;
  int startIndex = 0;
  int endIndex = 0;
  
  while(startIndex >= 0 && startIndex < message.length())
  {
    //Serial.println("START INDEX: " + String(startIndex));
    //Serial.println("END INDEX: " + String(endIndex));
    endIndex = message.indexOf(separator, startIndex);
    
    if(endIndex < 0)
      endIndex = message.length();
      
    returnVector.push_back(message.substring(startIndex, endIndex));
    startIndex = endIndex + 1;
    Serial.println(returnVector.back());
  }
  return returnVector;
}
BluetoothController::rxPackage BluetoothController::map(String type, String data, String ack)
{
	rxPackage package;
	/*String value = "";
  
	for (int i = 0; i < data.length(); i++)
	{
		char c = data.charAt(i);
		if (c == ioDataSeperator)
		{
			package.data.push_back(value.toInt());
			value = "";
			continue;
		}
		value += c;
	}*/

  vector<String> dataStr = split(data, ioDataSeperator);

  while(!dataStr.empty())
  {
    package.data.push_back(dataStr.back().toInt());
    dataStr.pop_back();
  }

  reverse(package.data.begin(), package.data.end());
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


//############## JONATHANS FUNCTIONS #################

// Message format: @VELOCITY;TURN;LIGHT;HONK;AUTODRIVE$    @(-255 - 255);(-255 - 255);1/0;1/0;1/0$
void BluetoothController::decodeMessage(String msg)
{
  int idx1 = msg.indexOf(';');
  String velocityStr = msg.substring(1, idx1);

  int idx2 = msg.indexOf(';', idx1 + 1);
  String turnStr = msg.substring(idx1 + 1, idx2);

  int idx3 = msg.indexOf(';', idx2 + 1);
  String lightStr = msg.substring(idx2 + 1, idx3);

  int idx4 = msg.indexOf(';', idx3 + 1);
  String honkStr = msg.substring(idx3 + 1, idx4);

  int idx5 = msg.indexOf('\0', idx4 + 1);
  String autodriveStr = msg.substring(idx4 + 1, idx5);

  /*Serial.println("VELOCITY:\t" + velocityStr);
  Serial.println("TURN:\t" + turnStr);
  Serial.println("LIGHT:\t" + lightStr);
  Serial.println("HONK:\t" + honkStr);
  Serial.println("AUTODRIVE:\t" + autodriveStr);*/

  AppInstructions* appInstructions = AppInstructions::getInstance();
  appInstructions->setInstructions(velocityStr.toInt(), turnStr.toInt(), lightStr.toInt(), honkStr.toInt(), autodriveStr.toInt());
  appInstructions->setInstructionsAvailable(true);
}

void BluetoothController::readBluetooth()
{
	String message;

	while(Serial.available() > 0)
	{
		delay(10);
		char currentChar = Serial.read();

		if(currentChar == '$')
			break;

		message += currentChar;
	}

	if(message.charAt(0) == '@')
		decodeMessage(message);
}
