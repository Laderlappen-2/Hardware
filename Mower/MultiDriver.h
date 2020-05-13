#pragma once
#include "AbsDriver.h"
//#include "LinkedList.h"
#include <ArduinoSTL.h>

class MultiDriver : protected AbsDriver
{
public:
	~MultiDriver();

	static MultiDriver* getInstance()
	{
		MultiDriver* instance = nullptr;
		if (instance == nullptr)
			instance = new MultiDriver();
		return instance;
	}

	void init();
	void addDriver(AbsDriver* driver, String name);
	void selectDriver(String name);

	void run() override;

protected:

	void sendCMD() override;
	void listener(int[], int) override;

	static void btListener(int data[], int size)
	{
		MultiDriver::getInstance()->listener(data, size);
	}

private:
	MultiDriver();

	struct driver_s
	{
		AbsDriver * driver;
		String nickname;
		inline bool operator==(const driver_s& d) const
		{
			return driver == d.driver && nickname == d.nickname;
		}
	};

	AbsDriver * selectedDriver;
	//typedef LinkedList<driver_s> driverList;
	typedef vector<driver_s> driverList;

	driverList drivers;
  #define STOP_IF_NULL if(selectedDriver == nullptr) return;
};
