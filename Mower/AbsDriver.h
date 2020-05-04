#include "Adrduino.h"
#include <stdlib.h>

class AbsDriver
{
public:

	AbsDriver() = 0;
	
	void run();

protected:
	
	void sendCMD();

	void listener(String);

private:

};