#ifndef __TASK_GAMEJAM_H__
#define __TASK_GAMEJAM_H__

class VoiceSegs;
class VoiceSegsQueue;

class TaskGameJam : public lw::Task, lw::ButtonCallback{
public:
	virtual void vBegin();
	virtual void vEnd();
	virtual void vMain(float dt);
	virtual void vDraw(float dt);
	virtual bool vOnTouchEvent(std::vector<lw::TouchEvent>& events);

private:
	void changeTargetPos();
	virtual void vOnClick(lw::UIButton* pButton);
	void reset();
	lw::GestureMgr* _pGesMgr;
	int _gesId;
	cml::Vector2 _gesPos;
	VoiceSegs* _segs_c_a001_01;
	VoiceSegs* _segs_c_a002_01;
	VoiceSegs* _segs_c_a003_01;
	VoiceSegs* _segs_c_b007_01;
	VoiceSegs* _segs_c_b008_01;
	VoiceSegs* _segs_c_b009_01;
	VoiceSegs* _segs_c_b010_01;
	VoiceSegs* _segs_c_b011_01;
	VoiceSegs* _segs_c_b012_01;
	VoiceSegsQueue* _pQueue;

	cml::Vector2 _targetPos;
	float _targetHP;
	float _manLife;
	float _volume;
	float _volumeTo;
	enum Step{
		STEP_PLAY,
		STEP_FINISH,
	};
	Step _step;
	float _flashAlpha;
	lw::Sprite* _pSptDeadMan;
	lw::Sprite* _pSptPointer;
	lw::Sprite* _pSptSitMan;
	lw::UIButton* _pBtnDieAgain;
	bool _showPoint;
	float _t;
	lw::Font* _pFontClock;
	bool _isTimeRun;
};

extern TaskGameJam taskGameJam;


#endif //__TASK_GAMEJAM_H__