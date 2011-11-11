#include "stdafx.h"
#include "app.h"
#include "taskGameJam.h"
#include "option.h"

MicroVol1App::MicroVol1App(){
	_config.orientation = lw::App::ORIENTATION_RIGHT;
	_config.maxFPS = 60;
	_config.title = L"Micro Vol1";

#ifdef WIN32
	lw::FileSys::addDirectory("data", true);
#endif

	int r = 0;
	r = sqlite3_open(_f("data.db"), &g_pDataDb);
	lwassert(r == SQLITE_OK);
#ifdef WIN32
	r = sqlite3_open(_f("save.db"), &g_pSaveDb);
	lwassert(r == SQLITE_OK);
#endif
#ifdef __APPLE__
	std::string docDir = lw::getDocDir();
	docDir += "/save.db";
	FILE* pf = fopen(docDir.c_str(), "rb");
	if ( pf == NULL ){
		pf = fopen(_f("save.db"), "rb");
		lwassert(pf);
		fseek(pf, 0, SEEK_END);
		int len = ftell(pf);
		char buf[len];
		fseek(pf, 0, SEEK_SET);
		fread(buf, len, 1, pf);
		fclose(pf);
		FILE* pOut = fopen(docDir.c_str(), "wb");
		lwassert(pOut);
		fwrite(buf, len, 1, pOut);
		fclose(pOut);
	}

	r = sqlite3_open(docDir.c_str(), &g_pSaveDb);
	lwassert(r == SQLITE_OK);
#endif

	taskGameJam.start(0);
	optionLoad();
	new lw::SoundMgr(5);
}

MicroVol1App::~MicroVol1App(){
	optionSave();
	sqlite3_close(g_pDataDb);
	sqlite3_close(g_pSaveDb);
	delete lw::SoundMgr::ps();
}

void MicroVol1App::vInit(){

}

void MicroVol1App::vQuit(){
	
}

void MicroVol1App::vMain(){
	lw::SoundMgr::s().main(25);
}