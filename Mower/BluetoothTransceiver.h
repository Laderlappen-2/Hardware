#include <SoftwareSerial.h>
#include <Arduino.h>

class BluetoothTransceiver
{
public:

  BluetoothTransceiver();
  BluetoothTransceiver(int baudrate);

  void sendDataExpectAck(String message);
  void sendAcknowledge();
  void setListener(void(*callback)(String));
  void run();

private:

  void(*_callback)(String);
  const int _txPin = 3;
  const int _rxPin = 2;
  SoftwareSerial _bluetoothModule = SoftwareSerial(_rxPin, _txPin);
  String _currentMessage;
  bool _acknowledgeReceived = true;
  unsigned long _acknowledgeTimeout = 0;
  
};
