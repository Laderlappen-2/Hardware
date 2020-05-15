#pragma once 
#include <MeEncoderOnBoard.h>
#include <math.h>

class EngineModule{

public:

#define NO_TURN 3001

    struct cmd
    {
        int speed;
        int turnRadius;
        int time_ms;
		inline void operator= (const cmd other)
		{
			speed = other.speed;
			turnRadius = other.turnRadius;
			time_ms = other.time_ms;
		}
		inline bool operator==(const cmd& c) const
		{
			return speed == c.speed && turnRadius == c.turnRadius && time_ms == c.time_ms;				
		}
    };
	struct point_s
	{
		int _x;
		int _y;
	};


	static EngineModule *getInstance()
	{
		static EngineModule* instance = nullptr;
		if (instance == nullptr)
			instance = new EngineModule();
		return instance;
	}
	void init(int, int);


  void run();
  void runOdometri();

	point_s getPosition();
  int getX();
  int getY();
  void updatePosition();

  void setCommand(cmd);

  void setTurn(int);

  void setSpeed(int);

  bool isReady();

  void stopp();

private:
  EngineModule();

  void execute_command(cmd*);

  void setWheels(int, int);

  static void isr_process_Wheel_Right(void)
  {
    if(digitalRead(getInstance()->Wheel_Right->getPortB()) == 0)
    {
      getInstance()->Wheel_Right->pulsePosMinus();
    }
    else
    {
      getInstance()->Wheel_Right->pulsePosPlus();;
    }
  }

  static void isr_process_Wheel_Left(void)
  {
    if(digitalRead(getInstance()->Wheel_Left->getPortB()) == 0)
    {
      getInstance()->Wheel_Left->pulsePosMinus();
    }
    else
    {
      getInstance()->Wheel_Left->pulsePosPlus();
    }
  }

	point_s position;

  MeEncoderOnBoard *Wheel_Right;
  MeEncoderOnBoard *Wheel_Left;
  bool _ready = true;

  cmd current_command;

	const int motorPWMmax = 255;
	const int motorPWMmin = -255;
  
	const float _wheelRadius_cm = 6.3 / 2;
  #define DEGREES_PER_CENTIMETER (360 / (_wheelRadius_cm * 2 * PI))
  const float _wheelToWheelGap_cm = 14.6;
	double _robotAngle_rad;

  const int _rightWheelOffset = 0;
  const int _leftWheelOffset = 0;
};
