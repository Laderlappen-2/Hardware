#include "EngineModule.h"


EngineModule::EngineModule()
{

}

void EngineModule::init(int slot1, int slot2)
{
	Wheel_Right = new MeEncoderOnBoard(slot1);
	Wheel_Left = new MeEncoderOnBoard(slot2);
}


void EngineModule::setCommand(cmd command)
{
	//current_command.speed = command.speed;
	//current_command.turnRadius = command.turnRadius;
	//current_command.time_ms = command.time_ms;
	current_command = command;
	_ready = false;
}

void EngineModule::setTurn(int value)
{
	//normalize value to [-255,255]
	value = constrain(value, -100, 100);
	value *= 2.55;

	//get the current pwm for each wheel
	int leftPWM = Wheel_Left->getCurPwm();
	int rightPWM = Wheel_Right->getCurPwm();

	//uptade difference between the wheels to match the new turn
	int diff = leftPWM - rightPWM;
	leftPWM += (value - diff) / 2;
	rightPWM -= (value - diff) / 2;

	//set the new pwm
	setWheels(leftPWM, rightPWM);
}

void EngineModule::setSpeed(int value)
{
	//get the current difference between the whels
	int turn = Wheel_Left->getCurPwm() - Wheel_Right->getCurPwm();
	//constraints the speed so that turn difference will remain the same
	//int speed = constrain(value, -255 + (turn / 2), 255 - (turn / 2));
  int speed = constrain(value, -100, 100);
  speed *= 2.55;

	//gets the current wheel pwm and updates it
	int leftPWM = Wheel_Left->getCurPwm() + speed;
	int rightPWM = Wheel_Right->getCurPwm() + speed;

	//set the new pwm
	setWheels(speed, speed);
}

bool EngineModule::isReady()
{
	return _ready;
}

void EngineModule::run()
{
	enum state_s
	{
		idle,
		update,
		wait,
		end,
	};

	Wheel_Right->loop();
	Wheel_Left->loop();
  //updatePosition();
	static state_s state = idle;
	static unsigned long startWait = 0;

	static cmd oldCmd = cmd{0,0,0};
	if (!(current_command == oldCmd))
	{
		state = update;
		oldCmd = current_command;
	}

	switch (state)
	{
	case idle:
		if (!isReady())
			state = update;
		break;
	case update:
		execute_command(&current_command);

		//if the time is negative, the cmd will run without a timelimit
		if (current_command.time_ms < 0)
		{
			_ready = true;
			state = idle;
			break;
		}

		startWait = millis();
		state = wait;
		break;
	case wait:
		if (millis() > startWait + current_command.time_ms)
		{
			_ready = true;
			state = idle;
		}
	case end:
		if (current_command.time_ms >= 0)
			stopp();
		state = idle;
		break;
	default:
		// TODO felhantering
		break;
	}
}

EngineModule::point_s EngineModule::getPosition()
{
	return position;
}

void EngineModule::execute_command(cmd *command)
{
	command->speed = constrain(command->speed, -255, 255);

	if (command->turnRadius == NO_TURN)
	{
    setWheels(command->speed, command->speed);
		//Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
		//Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
		return;
	}

	float rRight, rLeft;
	rRight = command->turnRadius - (_wheelToWheelGap_cm / 2);
	rLeft = command->turnRadius + (_wheelToWheelGap_cm / 2);

	if (command->turnRadius > 0)
	{
		float ratio = rRight / rLeft;
    setWheels(command->speed, command->speed * ratio);
		//Wheel_Right->setMotorPwm(-command->speed * ratio - _rightWheelOffset);
		//Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
	}

	else
	{
		float ratio = rLeft / rRight;
    setWheels(command->speed * ratio, command->speed);
		//Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
		//Wheel_Left->setMotorPwm(command->speed * ratio + _leftWheelOffset);
	}


}

void EngineModule::updatePosition()
{   
	static long oldAngleLeft =0;
	static long oldAngleRight = 0;
	// get the angular distance traveled by each wheel since the last update
	double leftDegrees = oldAngleLeft - Wheel_Left->getCurPos();
	double rightDegrees = oldAngleRight - Wheel_Right->getCurPos();
	oldAngleLeft = Wheel_Left->getCurPos();
	oldAngleRight = Wheel_Right->getCurPos();


	// convert the angular distances to linear distances
	double dLeft = leftDegrees / DEGREES_PER_MILLIMETER;
	double dRight = rightDegrees / DEGREES_PER_MILLIMETER;

	// calculate the length of the arc traveled by Colin
	double dCenter = (dLeft + dRight) / 2.0;

	// calculate Colin's change in angle
	double phi = (dRight - dLeft) / (double)_wheelToWheelGap_cm;
	// add the change in angle to the previous angle
	_robotAngle_rad += phi;
	// constrain _theta to the range 0 to 2 pi
	if (_robotAngle_rad > 2.0 * PI) _robotAngle_rad -= 2.0 * PI;
	if (_robotAngle_rad < 0.0) _robotAngle_rad += 2.0 * PI;

	// update Colin's x and y coordinates
	position._x += dCenter * cos(_robotAngle_rad);
	position._y += dCenter * sin(_robotAngle_rad);
}

void EngineModule::stopp()
{
  Serial.println(Wheel_Right->getCurPwm());
	Wheel_Right->setMotorPwm(0);
	Wheel_Left->setMotorPwm(0);
}

void EngineModule::setWheels(int left, int right)
{
  Wheel_Right->setMotorPwm(-right - _rightWheelOffset);
  Wheel_Left->setMotorPwm(left + _leftWheelOffset);
}
