#include "MultiDriver.h"



MultiDriver::MultiDriver()
{
}

MultiDriver::~MultiDriver()
{
	driverList* list = &drivers;
	while (!list->isEndOfList())
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
}

void MultiDriver::addDriver(AbsDriver * driver, String name)
{
	drivers.add(driver_s{ driver,name });
	//drivers.push_back(driver_s{ driver,name });
}

void MultiDriver::selectDriver(String name)
{
	driverList* list = &drivers;
	while (!list->isEndOfList())
	{
		if (list->getItem().nickname == name)
		{
			selectedDriver = list->getItem().driver;
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
	selectedDriver->run();
}

void MultiDriver::sendCMD()
{
	selectedDriver->sendCMD();
}

void MultiDriver::listener(int data[], int size)
{
	selectedDriver->listener(data, size);
}

