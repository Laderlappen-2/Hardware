#include "DriveController.h"

DriveController::DriveController()
{
}

void run()
{
	switch (driveMode) 
	{
	case none:
  {
    //Do nothing.
		break;
  }
  
	case automatic:
  {
    //Call a run() function in AutoDriver? autoDriver.run();
		break;
  }
  
	case manual:
  {
    //Execute incoming commands on Bluetooth.
		break;
  }
  
  default:
    break;
	}
}

void DriveController::manualListener(int[], int)
{
  
}

void DriveController::autoListener(int[], int)
{
  
}

void setDriveMode(drive_m mode)
{
  driveMode = mode;
}

drive_m getDriveMode()
{
  return driveMode;
}
