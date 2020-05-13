#include "MultiDriver.h"
#include "BluetoothController.h"
 

MultiDriver::MultiDriver() 
{
  selectedDriver = nullptr;
}

MultiDriver::~MultiDriver()
{
	driverList* list = &drivers;
	while (list != nullptr)
	{
		delete list->getItem().driver;
		list = list->getNext();
	}
	//while (!drivers.empty())
	//{
	//	delete drivers.back().driver;
	//	drivers.pop_back();
	//}
}

void MultiDriver::init()
{
	//TODO link bt listner
	BluetoothController::getInstance()->addReciveListner(BluetoothController::reciveType_e::drive,btListener);
	AbsDriver::init();
}

void MultiDriver::addDriver(AbsDriver * driver, String name)
{
	drivers.add(driver_s{ driver,name });
	//drivers.push_back(driver_s{ driver,name });
}

void MultiDriver::selectDriver(String name)
{
  Serial.println("MULTIDRIVER NAME: " + name);
	driverList* list = &drivers;
	while (list!= nullptr)
	{
    Serial.println("ITEM: " + list->getItem().nickname);
		if (list->getItem().nickname == name)
		{
      Serial.println("FOUND DRIVER!");
			selectedDriver->onDeactivation();
			selectedDriver = list->getItem().driver;
			selectedDriver->onActivation();

			break;
		}
		list = list->getNext();
	}

	//for (int i = 0; i < drivers.size(); i++)
	//{
	//	if (drivers.at(i).nickname == name)
	//	{
	//		selectedDriver = drivers.at(i).driver;
	//		break;
	//	}
	//}
}

void MultiDriver::run()
{
  STOP_IF_NULL;
	selectedDriver->run();
}


void MultiDriver::sendCMD()
{
  STOP_IF_NULL;
	selectedDriver->sendCMD();
}

void MultiDriver::listener(int data[], int size)
{
  STOP_IF_NULL;
	selectedDriver->listener(data, size);
}
