#include "EngineModule.h"

EngineModule::EngineModule() {}

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
	value = (value / 100.0f) * 255;

	//get the current pwm for each wheel
	int leftPWN = Wheel_Left->getCurPwm();
	int rightPWM = Wheel_Right->getCurPwm();

	//uptade difference between the wheels to match the new turn
	int diff = leftPWN - rightPWM;
	leftPWN += (value - diff) / 2;
	rightPWM -= (value - diff) / 2;

	//set the new pwm
	Wheel_Left->setMotorPwm(leftPWN);
	Wheel_Right->setMotorPwm(rightPWM);
}

void EngineModule::setSpeed(int value)
{
	//get the current difference between the whels
	int turn = Wheel_Left->getCurPwm() - Wheel_Right->getCurPwm();
	//constraints the speed so that turn difference will remain the same
	int speed = constrain(value, -255 + (turn / 2), 255 - (turn / 2));

	//gets the current wheel pwm and updates it
	int leftPWM = Wheel_Left->getCurPwm() + speed;
	int rightPWM = Wheel_Right->getCurPwm() + speed;

	//set the new pwm
	Wheel_Left->setMotorPwm(leftPWM);
	Wheel_Right->setMotorPwm(rightPWM);
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
	};

	Wheel_Right->loop();
	Wheel_Left->loop();
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
		{
			state = update;
		}
		else if (current_command.time_ms >= 0)
			stopp();
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
		break;
	default:
		// TODO felhantering
		break;
	}
}

void EngineModule::execute_command(cmd *command)
{
	command->speed = constrain(command->speed, -255, 255);

	if (command->turnRadius == NO_TURN)
	{
		Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
		Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
		return;
	}

	float rRight, rLeft;
	rRight = command->turnRadius - (_wheelToWheelGap / 2);
	rLeft = command->turnRadius + (_wheelToWheelGap / 2);

	if (command->turnRadius > 0)
	{
		float ratio = rRight / rLeft;
		Wheel_Right->setMotorPwm(-command->speed * ratio - _rightWheelOffset);
		Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
	}

	else
	{
		float ratio = rLeft / rRight;
		Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
		Wheel_Left->setMotorPwm(command->speed * ratio + _leftWheelOffset);
	}


}

void EngineModule::stopp()
{
	Wheel_Right->setMotorPwm(0);
	Wheel_Left->setMotorPwm(0);
}
