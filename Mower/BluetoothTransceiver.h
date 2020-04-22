#include <SoftwareSerial.h>
#include <Arduino.h>

class BluetoothTransceiver
{
public:

  BluetoothTransceiver();
  BluetoothTransceiver(int baudrate);

  void sendData(String message);
  void rxListener(void(*callback)(String));
  void run();

private:

  void(*_callback)(String);
  const int _txPin = 3;
  const int _rxPin = 2;
  SoftwareSerial _bluetoothModule = SoftwareSerial(_rxPin, _txPin);
  
};
