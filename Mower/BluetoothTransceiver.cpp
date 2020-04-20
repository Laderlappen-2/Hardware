#include "BluetoothTransceiver.h"

BluetoothTransceiver::BluetoothTransceiver(){}

BluetoothTransceiver::BluetoothTransceiver(int baudrate)
{
  bluetoothModule.begin(baudrate);
}

void BluetoothTransceiver::send(String message)
{
  
}

void BluetoothTransceiver::setListener(void(*callback)(String))
{
  
}
