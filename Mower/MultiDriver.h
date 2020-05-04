#pragma once
#include "AbsDriver.h"
#include "LinkedList.h"


class MultiDriver : protected AbsDriver
{
public:
	MultiDriver();
	~MultiDriver();

	void init();
	void addDriver(AbsDriver* driver, String name);
	void selectDriver(String name);

	void run() override;

protected:

	void sendCMD() override;
	void listener(int[], int) override;

private:

	struct driver_s
	{
		AbsDriver * driver;
		String nickname;
	};

	AbsDriver * selectedDriver;
	typedef LinkedList<driver_s> driverList;
	driverList drivers;
};

