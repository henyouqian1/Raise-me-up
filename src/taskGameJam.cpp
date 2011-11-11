#include "stdafx.h"
#include "TaskGameJam.h"
#include "voice.h"
#include "geomDraw.h"
#include "clock.h"

namespace{
	const float c_a001_01[] = {
		2.107f, 4.558f, 7.179f, 9.791f, 12.190f,
	};
	const float c_a002_01[] = {
		1.589f, 3.004f, 4.527f, 5.909f, 7.248f, 8.619f, 9.816f, 11.274f, 13.026f,
	};
	const float c_a003_01[] = {
		1.005f, 2.117f, 3.399f, 5.219f, 6.432f,
	};
	const float c_b007_01[] = {
		1.794f, 2.886f, 4.152f, 5.371f, 7.001f, 8.458f, 9.803f, 11.354f, 12.541f, 13.839f, 15.264f, 17.069f, 18.947f,
	};
	const float c_b008_01[] = {
		1.199f, 2.200f, 3.356f, 3.935f, 5.091f, 6.262f, 7.305f, 8.433f, 9.801f, 10.676f, 11.874f, 12.918f, 13.905f, 15.005f, 16.119f, 16.881f,
	};
	const float c_b009_01[] = {
		1.683f, 2.949f, 4.260f, 5.437f, 6.599f, 7.552f, 8.491f, 9.534f, 10.770f, 11.947f, 13.153f, 14.062f, 15.254f, 16.282f, 17.831f,
	};
	const float c_b010_01[] = {
		1.232f, 2.347f, 3.419f, 4.521f, 5.607f, 6.636f, 7.737f, 8.795f, 9.766f, 10.809f, 12.707f, 13.794f, 14.750f, 15.909f, 17.343f, 
	};
	const float c_b011_01[] = {
		1.067f, 2.286f, 3.399f, 4.176f, 5.167f, 5.990f, 6.905f, 7.651f, 8.673f, 9.572f, 10.380f, 11.172f, 11.904f, 12.544f, 13.337f, 14.144f, 14.861f, 15.684f, 16.446f, 17.269f, 18.244f
	};
	const float c_b012_01[] = {
		2.535f, 4.887f, 7.013f, 10.858f, 13.816f, 16.858f, 
	};

	const float TARGET_RADIUS = 80.f;
	const float TARGET_BOTTOM_MARGIN = 30.f;
	const float TARGET_HP = 1.f;

	const int pointerUVs[8*2] = {
		0, 0, 80, 0, 160, 0, 240, 0, 320, 0, 400, 0, 0, 50, 80, 50,
	};
	const int pointerW = 80;
	const int pointerH = 50;
}

void TaskGameJam::vBegin(){
	lw::srand();
	lw::App::s().setFrameInterval(1);
	lw::App::s().setOrient(lw::App::ORIENTATION_RIGHT);
	_pGesMgr = new lw::GestureMgr();

	_segs_c_a001_01 = new VoiceSegs("c_a001_01.wav", c_a001_01, sizeof(c_a001_01)/sizeof(float));
	_segs_c_a002_01 = new VoiceSegs("c_a002_01.wav", c_a002_01, sizeof(c_a002_01)/sizeof(float));
	_segs_c_a003_01 = new VoiceSegs("c_a003_01.wav", c_a003_01, sizeof(c_a003_01)/sizeof(float), false);
	_segs_c_b007_01 = new VoiceSegs("c_b007_01.wav", c_b007_01, sizeof(c_b007_01)/sizeof(float));
	_segs_c_b008_01 = new VoiceSegs("c_b008_01.wav", c_b008_01, sizeof(c_b008_01)/sizeof(float));
	_segs_c_b009_01 = new VoiceSegs("c_b009_01.wav", c_b009_01, sizeof(c_b009_01)/sizeof(float));
	_segs_c_b010_01 = new VoiceSegs("c_b010_01.wav", c_b010_01, sizeof(c_b010_01)/sizeof(float));
	_segs_c_b011_01 = new VoiceSegs("c_b011_01.wav", c_b011_01, sizeof(c_b011_01)/sizeof(float));
	_segs_c_b012_01 = new VoiceSegs("c_b012_01.wav", c_b012_01, sizeof(c_b012_01)/sizeof(float));

	_targetPos[0] = 10000.f;
	_targetPos[1] = 10000.f;
	_pQueue = new VoiceSegsQueue();

	_pSptDeadMan = lw::Sprite::create("gameJam.png");
	_pSptDeadMan->setUV(160, 50, 338, 69);
	_pSptDeadMan->setPos(80.f, 250.f);

	_pSptPointer = lw::Sprite::create("gameJam.png");
	_pSptPointer->setUV(0, 0, pointerW, pointerH);
	_pSptPointer->setPos(80+202-42, 250+28-40);

	_pSptSitMan = lw::Sprite::create("gameJam.png");
	_pSptSitMan->setUV(0, 119, 232, 213);
	_pSptSitMan->setPos(192, 107);

	 lw::Button9Def def("ui.png", 0, 0, 0, 2, 0, 0, 1, 1, 1, 1, 1, 1, "calibri20.fnt", GL_NEAREST); 
	_pBtnDieAgain = lw::UIButton::create9(def);
	_pBtnDieAgain->setPos(120, 100);
	_pBtnDieAgain->setSize(80, 40);
	_pBtnDieAgain->setColor(lw::COLOR_WHITE, lw::COLOR_WHITE, lw::COLOR_WHITE);
	_pBtnDieAgain->setTextColor(lw::COLOR_WHITE);
	_pBtnDieAgain->setText(L"Die again?");
	_pBtnDieAgain->setCallback(this);

	_showPoint = false;

	_pFontClock = lw::Font::create("calibri20.fnt");
	_pFontClock->setAlign(lw::ALIGN_TOP_LEFT);
	_pFontClock->setPos(350.f, 5.f);
	
	reset();
}

void TaskGameJam::vEnd(){
	lw::App::s().popOrient();
	delete _segs_c_a001_01;
	delete _segs_c_a002_01;
	delete _segs_c_a003_01;
	delete _segs_c_b007_01;
	delete _segs_c_b008_01;
	delete _segs_c_b009_01;
	delete _segs_c_b010_01;
	delete _segs_c_b011_01;
	delete _segs_c_b012_01;
	delete _pGesMgr;
	delete _pSptDeadMan;
	delete _pSptPointer;
	delete _pSptSitMan;
	delete _pFontClock;
}

void TaskGameJam::vMain(float dt){
	if ( _step == STEP_PLAY ){
		if ( _isTimeRun ){
			_t += 1000.f/60.f;
		}
		
		std::wstring s;
		Clock::toString(s, (int)_t);
		_pFontClock->setText(s.c_str());
		_pQueue->main();
		_manLife -= 0.0005f;
		if ( _gesId != -1 ){
			cml::Vector2 v = _targetPos - _gesPos;
			float len = v.length();
			_volumeTo = max((1.f-len/TARGET_RADIUS*.5f), 0.f);
			_manLife += 0.002f*_volumeTo;
			_targetHP -= .005f*_volumeTo;
			if ( _targetHP <= 0.f ){
				_targetHP = TARGET_HP;
				changeTargetPos();
				_flashAlpha = 0.01f;
			}
		}else{
			if ( _volumeTo > .2f ){
				_volumeTo -= 0.005f;
			}
		}
		float dv = 0.01f;
		if ( fabs(_volume-_volumeTo) <= dv ){
			_volume = _volumeTo;
		}else{
			if ( _volume > _volumeTo ){
				_volume -= dv;
			}else{
				_volume += dv;
			}
		}
		_pQueue->setVolume(_volume);
		_manLife = max(0.f, _manLife);
		int n = (int)(_manLife*8.f);
		n = min(7, n);

		_pSptPointer->setUV(pointerUVs[n*2], pointerUVs[n*2+1],  pointerW, pointerH);
		if ( _manLife < .25f ){
			_pQueue->add(_segs_c_a002_01);
		}else if ( _manLife < .5f ){
			_pQueue->add(_segs_c_b007_01);
		}else if ( _manLife < .75f ){
			_pQueue->add(_segs_c_b009_01);
		}else{
			_pQueue->add(_segs_c_b011_01);
		}
		if ( _flashAlpha > 0.f ){
			_flashAlpha += .1f;
			if ( _flashAlpha > 1.1f ){
				_flashAlpha = 0.f;
			}
		}
		if ( _manLife >= 1.f ){
			_step = STEP_FINISH;
			_pBtnDieAgain->show(true);
			_flashAlpha = 2.f;
		}
	}else if ( _step == STEP_FINISH ){
		_volume -= 0.005f;
		_pQueue->setVolume(_volume);
		_flashAlpha -= 0.01f;
	}
}

void TaskGameJam::vDraw(float dt){
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if ( _step == STEP_PLAY ){
		_pSptDeadMan->collect();
		_pSptPointer->collect();
		_pFontClock->collect();
		lw::Sprite::flush();
		geomDrawBegin();
		if ( _showPoint ){
			geomDrawCircle(_targetPos[0], _targetPos[1], TARGET_RADIUS, lw::COLOR_NONE, lw::COLOR_WHITE);
		}
		if ( _flashAlpha > 0.f ){
			lw::Color clr = lw::Color(1.f, 1.f, 1.f, _flashAlpha);
			geomDrawCube(0.f, 0.f, 480.f, 320.f, clr, clr);
		}
		geomDrawEnd();
	}else if ( _step == STEP_FINISH ){
		_pSptSitMan->collect();
		_pFontClock->collect();
		lw::Sprite::flush();
		geomDrawBegin();
		if ( _flashAlpha > 0.f ){
			lw::Color clr = lw::Color(1.f, 1.f, 1.f, _flashAlpha);
			geomDrawCube(0.f, 0.f, 480.f, 320.f, clr, clr);
		}
		geomDrawEnd();
	}
}

bool TaskGameJam::vOnTouchEvent(std::vector<lw::TouchEvent>& events){
	_pGesMgr->onTouchEvent(events);
	const std::list<lw::Gesture>& gestures = _pGesMgr->getGestures();
	std::list<lw::Gesture>::const_iterator it = gestures.begin();
	std::list<lw::Gesture>::const_iterator itEnd = gestures.end();
	for ( ;it != itEnd; ++it ){
		if ( it->updated ){
			const lw::TouchEvent& evt = it->evt;
			if ( evt.type == lw::TouchEvent::TOUCH ){
                if ( evt.x < 20 && evt.y < 20 ){
					_showPoint = !_showPoint;
				}else if ( _gesId == -1 ){
                    _gesId = it->id;
                    _gesPos[0] = evt.x;
                    _gesPos[1] = evt.y;
                }
				
				
				_isTimeRun = true;
			}else if ( evt.type == lw::TouchEvent::MOVE ){
				if ( _gesId == it->id ){
					_gesPos[0] = evt.x;
					_gesPos[1] = evt.y;
				}
			}else if ( evt.type == lw::TouchEvent::UNTOUCH ){
				_gesId = -1;
			}
		}
	}
	_pGesMgr->main();
	return false;
}

void TaskGameJam::vOnClick(lw::UIButton* pButton){
	if ( pButton == _pBtnDieAgain ){
		reset();
	}
}

void TaskGameJam::reset(){
	_pBtnDieAgain->show(false);
	_step = STEP_PLAY;
	_gesId = -1;
	_targetHP = TARGET_HP;
	_manLife = 0.f;
	_volume = 0.f;
	_volumeTo = 0.f;
	_flashAlpha = 0.f;
	_t = 0.f;
	_isTimeRun = false;

	changeTargetPos();
}

void TaskGameJam::changeTargetPos(){
	cml::Vector2 vOld = _targetPos;
	cml::Vector2 v;
	while (1){
		_targetPos[0] = cml::random_float(TARGET_RADIUS, 480.f-TARGET_RADIUS);
		_targetPos[1] = cml::random_float(TARGET_RADIUS, 320.f-TARGET_RADIUS-TARGET_BOTTOM_MARGIN);
		v = _targetPos - vOld;
		if ( v.length() > 1.6f* TARGET_RADIUS ){
			break;
		}
	}
}

TaskGameJam taskGameJam;