#include "AutoDriver.h"
#include "ManualDriver.h"

class DriveController
{
public:

	DriveController();
  
	void run();

private:

  enum drive_m
  {
    none,
    automatic,
    manual
  };
  drive_m driveMode = none;

  void setDriveMode(drive_m mode);
  
  void manualListener(int[], int);
  void autoListener(int[], int);
  
	AutoDriver autoDriver;
	ManualDriver manualDriver;

};
