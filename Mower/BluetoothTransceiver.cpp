#include "BluetoothTransceiver.h"

BluetoothTransceiver::BluetoothTransceiver(){}

BluetoothTransceiver::BluetoothTransceiver(int baudrate)
{
  _bluetoothModule.begin(baudrate);
}

void BluetoothTransceiver::sendData(String message)
{
  if(_callback != nullptr)
  {
    _callback(message);
  }

  //_bluetoothModule.print(message + "\r");   Code for sending data to the app.
}

void BluetoothTransceiver::rxListener(void(*callback)(String))
{
  _callback = callback;
}

void BluetoothTransceiver::run()
{
  enum state_e
  {
    idle,
    receive
  };
  static state_e state = idle;

  switch(state)
  {
    case idle:
    {
      if(_bluetoothModule.available())  //Checks if there is any data available.
      {
        state = receive;
        break;
      }
      
      break;
    }

    case receive:
    {
      String message = "";
      while(_bluetoothModule.available())
      {
        char currentChar = _bluetoothModule.read();
        message += currentChar;
        
        if(currentChar == '$')
          break;
      }
      
      if(_callback != nullptr && message.startsWith("@"))   //Maybe check the @ sign somewhere else to make sure that the message is in correct format?
        _callback(message);   //Send message to BluetoothController. Callback function in BluetoothController?
        
      state = idle;
      break;
    }
    
    default:
      break;
  }
  
  
}
