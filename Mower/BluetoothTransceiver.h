#include <SoftwareSerial.h>

class BluetoothTransceiver
{
public:

  BluetoothTransceiver();
  BluetoothTransceiver(int baudrate);

  void send(String message);
  void setListener(void(*callback)(String));

private:

  String dataReceived;
  const int txPin;    //Must be initiated here
  const int rxPin;    //Must be initiated here
  SoftwareSerial bluetoothModule = SoftwareSerial(txPin, rxPin);
  
};
