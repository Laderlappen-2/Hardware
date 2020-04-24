#include "BluetoothTransceiver.h"

BluetoothTransceiver::BluetoothTransceiver(){}

BluetoothTransceiver::BluetoothTransceiver(int baudrate)
{
  _bluetoothModule.begin(baudrate);
}

void BluetoothTransceiver::sendDataExpectAck(String message)
{
  _currentMessage = message;
  _bluetoothModule.print(message + "\r");   //Code for sending data to the app.
  _acknowledgeTimeout = millis() + 2000;
  _acknowledgeReceived = false;
}

void BluetoothTransceiver::sendAcknowledge()
{
  _bluetoothModule.print("@A$\r");   //Code for sending data to the app.
}

void BluetoothTransceiver::setListener(void(*callback)(String))
{
  _callback = callback;
}

void BluetoothTransceiver::run()
{
  enum state_e
  {
    idle,
    receive,
    resend
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

      else if(!_acknowledgeReceived && millis() > _acknowledgeTimeout)
      {
        state = resend;
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

        if(message == "@A$")   //Acknowledge message format.
        {
          _acknowledgeReceived = true;
          state = idle;
          return;
        }
        
        if(message.startsWith('@') && message.endsWith('$'))
          break;
      }

      if(message.charAt(message.length() - 2) == '1')   //Read acknowledge flag.
        sendAcknowledge();
      
      if(_callback != nullptr)
        _callback(message);   //Send message to BluetoothController. Callback function in BluetoothController?
        
      state = idle;
      break;
    }

    case resend:  //Only enter if we have not received acknowledge. (Resending data).
    {
      sendDataExpectAck(_currentMessage);
      state = idle;
      break;
    }
    
    default:
      break;
  }
  
  
}
