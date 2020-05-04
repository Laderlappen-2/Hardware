#include <Arduino.h>
#include <stdlib.h>

class AbsDriver
{
public:

	virtual void init() = 0;
	
	virtual	void run();

protected:
	
	virtual	void sendCMD();

	virtual	void listener(String);

private:

};