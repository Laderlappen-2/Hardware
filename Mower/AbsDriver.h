#include "Adrduino.h"
#include <stdlib.h>

class AbsDriver
{
public:

	AbsDriver() = 0;

	void sendCMD();

	void run();

	void listener(String);
private:

};