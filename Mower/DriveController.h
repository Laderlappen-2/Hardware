#include AutoDriver.h
#include ManualDriver.h

class DriveController() 
{
public:

	DriveController();

	void run();

private:

	enum drive_m
	{
		none,
		auto,
		manual,
	};
	Static drive_m driveMode = none;

	AutoDriver autoDriver;
	ManualDriver manualDriver;

}