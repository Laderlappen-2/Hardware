#include <Arduino.h>
#include <stdlib.h>

class AbsDriver
{
public:
	 
	virtual void init() = 0;
	
	virtual	void run();
	virtual	void sendCMD();
	virtual	void listener(int[],int);

protected:
	

private:

};