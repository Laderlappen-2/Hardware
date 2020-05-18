#include "EngineModule.h"


EngineModule::EngineModule()
{
}

void EngineModule::init(int slot1, int slot2)
{
	Wheel_Right = new MeEncoderOnBoard(slot1);
	Wheel_Left = new MeEncoderOnBoard(slot2);

  attachInterrupt(Wheel_Right->getIntNum(), isr_process_Wheel_Right, RISING);
  attachInterrupt(Wheel_Left->getIntNum(), isr_process_Wheel_Left, RISING);

  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  Wheel_Right->setPulse(9);
  Wheel_Left->setPulse(9);
  Wheel_Right->setRatio(19.792);
  Wheel_Left->setRatio(19.792);
  Wheel_Right->setPosPid(1.8,0,1.2);
  Wheel_Left->setPosPid(1.8,0,1.2);
  Wheel_Right->setSpeedPid(0.18,0,0);
  Wheel_Left->setSpeedPid(0.18,0,0);
}


void EngineModule::setCommand(cmd command)
{
	current_command = command;
	_ready = false;
}

void EngineModule::setSpeed(int value)
{ 
    value *= 2.55;
    //get the current difference between the wheels
    int turn = Wheel_Right->getCurPwm() - Wheel_Left->getCurPwm();
    //constraints the speed so that turn difference will remain the same
    int speed = constrain(value, motorPWMmin + (turn / 2), motorPWMmax - (turn / 2));

    //gets the current wheel pwm and updates it
    int leftPWM =  speed - turn/2;
    int rightPWM = speed + turn/2;

    //set the new pwm
    setWheels(leftPWM, rightPWM);
}

void EngineModule::setTurn(int value)
{
    //normalize value to [-255,255]
    value = (int)(value * 2.55f);
    value = constrain(value, motorPWMmin, motorPWMmax);
    
    //get the current pwm for each wheel
    int leftPWM = Wheel_Left->getCurPwm();
    int rightPWM = Wheel_Right->getCurPwm();
    int speed = (-rightPWM + leftPWM) / 2;

    //Adjusting steer sensitivity
    value /= 4;
    //uptade difference between the wheels to match the new turn
    leftPWM = speed + value;
    rightPWM = speed - value;

    //adjust for overshoot
    leftPWM = constrain(leftPWM, motorPWMmin, motorPWMmax);
    rightPWM = constrain(rightPWM, motorPWMmin, motorPWMmax);

    //set the new pwm
    setWheels(leftPWM, rightPWM);
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

		startWait = millis();
		state = wait;
		break;
	case wait:
		if (millis() > startWait + current_command.time_ms)
		{
			_ready = true;
			state = end;
		}
	case end:
		if (current_command.time_ms >= 0)
			stopp();
		state = idle;
		break;
	default:
		break;
	}
}

void EngineModule::runOdometri()
{
  Wheel_Left->updateCurPos();
  Wheel_Right->updateCurPos();
	Wheel_Right->loop();
	Wheel_Left->loop();
	updatePosition();
}

EngineModule::point_s EngineModule::getPosition()
{
	return position;
}

float EngineModule::getX()
{
  return position._x;
}

float EngineModule::getY()
{
  return position._y;
}

void EngineModule::execute_command(cmd *command)
{
	command->speed = constrain(command->speed, -255, 255);

	if (command->turnRadius == NO_TURN)
	{
    setWheels(command->speed, command->speed);
		return;
	}

	float rRight, rLeft;
	rRight = command->turnRadius - (_wheelToWheelGap_cm / 2);
	rLeft = command->turnRadius + (_wheelToWheelGap_cm / 2);

	if (command->turnRadius > 0)
	{
		float ratio = rRight / rLeft;
    setWheels(command->speed, command->speed * ratio);
	}

	else
	{
		float ratio = rLeft / rRight;
    setWheels(command->speed * ratio, command->speed);
	}


}

void EngineModule::updatePosition()
{
	static long oldAngleLeft =0;
	static long oldAngleRight = 0;
	// get the angular distance traveled by each wheel since the last update
	double leftDegrees = oldAngleLeft + Wheel_Left->getCurPos();
	double rightDegrees = oldAngleRight - Wheel_Right->getCurPos();
 
	oldAngleLeft = -Wheel_Left->getCurPos();
	oldAngleRight = Wheel_Right->getCurPos();

	// convert the angular distances to linear distances
	double dLeft = leftDegrees / DEGREES_PER_CENTIMETER;
	double dRight = rightDegrees / DEGREES_PER_CENTIMETER;

	// calculate the length of the arc traveled by Colin
	double dCenter = (dLeft + dRight) / 2.0;

	// calculate Colin's change in angle
	double phi = (dRight - dLeft) / _wheelToWheelGap_cm;
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
	Wheel_Right->setMotorPwm(0);
	Wheel_Left->setMotorPwm(0);
}

void EngineModule::setWheels(int left, int right)
{
  Wheel_Right->setMotorPwm(-right - _rightWheelOffset);
  Wheel_Left->setMotorPwm(left + _leftWheelOffset);
}
