#include "BluetoothController.h"
#include "EngineModule.h"
#include <Arduino.h>


BluetoothController::BluetoothController()
{
}

void BluetoothController::init(int baud)
{
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
	};
	static state_e state = idle;
	static const int waitTimeout_ms = 3000;
  static const int waitTimeoutSend = 2000;
  static unsigned long startWaitSend = 0;
	static long startWait = 0;
	static String message = "";
  EngineModule *engine = EngineModule::getInstance();

	switch (state)
	{
	case idle:
    if(millis() > startWaitSend + waitTimeoutSend)
    {
      state = sending;
    }
		break;
    
	case sending:
    String x = String(engine->getX());
    String y = String(engine->getY());
    Serial.print("@0," + x + ";" + y + "$");
    startWaitSend = millis();
    state = idle;
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
  }
  
  if(message == "")
    return;
  
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

void BluetoothController::send(EventType_e type, float xPos, float yPos)
{
	sendBuffer.enqueue(
		ioStart + String(type) + 
		ioSeperator + String(xPos) +
		ioDataSeperator + String(yPos) +
		ioSeperator + String(millis()) +
		ioEnd);
}

void BluetoothController::addReciveListner(reciveType_e type, void(*callback)(int[], int))
{
	if (callback == nullptr)
	{
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
	vector<String> subMessage = split(message, ioSeperator);
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
  int startIndex = message.indexOf(ioStart);
  int endIndex = message.indexOf(ioEnd);
	return (message.substring(startIndex + 1, endIndex));
}

vector<String> BluetoothController::split(String message, char separator)
{	
  vector<String> returnVector;
  int startIndex = 0;
  int endIndex = 0;
  
  while(startIndex >= 0 && startIndex < message.length())
  {
    endIndex = message.indexOf(separator, startIndex);
    
    if(endIndex < 0)
      endIndex = message.length();
      
    returnVector.push_back(message.substring(startIndex, endIndex));
    startIndex = endIndex + 1;
  }
  return returnVector;
}

BluetoothController::rxPackage BluetoothController::map(String type, String data, String ack)
{
	rxPackage package;
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
