#ifndef __APP_H__
#define __APP_H__

#include "lwTinyFrame/lwApp.h"

class MicroVol1App : public lw::App{
public:
	MicroVol1App();
	~MicroVol1App();

private:
	virtual void vInit();
	virtual void vQuit();
	virtual void vMain();
};

#endif //__APP_H__