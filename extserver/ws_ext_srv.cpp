
#include "extserver.h"

int main(int argc, char *argv[])
{
	extserver app;
	app.init(argc, argv);
	//getchar();
	app.run();
	app.term();
	return 0;
}
