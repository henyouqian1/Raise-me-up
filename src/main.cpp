#include "stdafx.h"
#include "app.h"
//#include <vld.h>

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(2030);

	MicroVol1App app;
	app.init();
	app.run();
	app.quit();
	return 0;
}

