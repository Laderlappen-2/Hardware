#include "MultiDriver.h"
#include "BluetoothController.h"
 

MultiDriver::MultiDriver() 
{
  selectedDriver = nullptr;
}

MultiDriver::~MultiDriver()
{
	while (!drivers.empty())
	{
		delete drivers.back().driver;
		drivers.pop_back();
	}
}

void MultiDriver::init()
{
	BluetoothController::getInstance()->addReciveListner(BluetoothController::reciveType_e::drive,btListener);
	AbsDriver::init();
}

void MultiDriver::addDriver(AbsDriver * driver, String name)
{
	drivers.push_back(driver_s{ driver,name });
}

void MultiDriver::selectDriver(String name)
{
  for(int i = 0; i < drivers.size(); i++)
  {
    if(drivers.at(i).nickname == name)
    {
      selectedDriver = drivers.at(i).driver;
    }
  }
}

void MultiDriver::run()
{
  RETURN_IF_NULL;
	selectedDriver->run();
}

void MultiDriver::sendCMD()
{
  RETURN_IF_NULL;
	selectedDriver->sendCMD();
}

void MultiDriver::listener(int data[], int size)
{
  RETURN_IF_NULL;
	selectedDriver->listener(data, size);
}
