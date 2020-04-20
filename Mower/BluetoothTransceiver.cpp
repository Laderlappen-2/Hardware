#include "BluetoothTransceiver.h"

BluetoothTransceiver::BluetoothTransceiver(){}

BluetoothTransceiver::BluetoothTransceiver(int baudrate)
{
  bluetoothModule.begin(baudrate);
}

void BluetoothTransceiver::sendData(String message)
{
  bluetoothModule.print(message + "\r");
}

void BluetoothTransceiver::rxListener(void(*callback)(String))
{
  
}
