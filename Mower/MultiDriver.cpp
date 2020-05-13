#include "MultiDriver.h"
#include "BluetoothController.h"
 

MultiDriver::MultiDriver() 
{
  selectedDriver = nullptr;
}

MultiDriver::~MultiDriver()
{
	//driverList* list = &drivers;
	//while (list != nullptr)
	//{
	//	delete list->getItem().driver;
	//	list = list->getNext();
	//}
	while (!drivers.empty())
	{
		delete drivers.back().driver;
		drivers.pop_back();
	}
}

void MultiDriver::init()
{
	//TODO link bt listner
	BluetoothController::getInstance()->addReciveListner(BluetoothController::reciveType_e::drive,btListener);
	AbsDriver::init();
}

void MultiDriver::addDriver(AbsDriver * driver, String name)
{
	//drivers.add(driver_s{ driver,name });
	drivers.push_back(driver_s{ driver,name });
}

void MultiDriver::selectDriver(String name)
{
	/*for (int i = 0; i < drivers.size(); i++)
	{
		if (drivers.at(i).nickname == name)
		{
			if (selectedDriver != nullptr)
				selectedDriver->onDeactivation();
			selectedDriver = drivers.at(i).driver;
			selectedDriver->onActivation();
			break;
		}
	}*/

  for(int i = 0; i < drivers.size(); i++)
  {
    if(drivers.at(i).nickname == name)
    {
      selectedDriver = drivers.at(i).driver;
      Serial.println(String(drivers.at(i).nickname) + " SELECTED");
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
