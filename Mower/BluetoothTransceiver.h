#include <SoftwareSerial.h>

class BluetoothTransceiver
{
public:

  BluetoothTransceiver();
  BluetoothTransceiver(int baudrate);

  void sendData(String message);
  void rxListener(void(*callback)(String));

private:

  void(*_callback)(String);
  const int _txPin;    //Must be initiated here
  const int _rxPin;    //Must be initiated here
  SoftwareSerial bluetoothModule = SoftwareSerial(_txPin, _rxPin);
  
};
