#ifndef __VOICE_H__
#define __VOICE_H__

class VoiceSegs{
public:
	VoiceSegs(const char* file, const float* firstSegPoint, int size, bool isPlayAnyPoint = true);
	~VoiceSegs();
	void play();
	void playAnyPoint();
	float getNextPoint();
	lw::Sound* getSound(){
		return _pSound;
	}

private:
	const float* _firstSegPoint;
	int _size;
	lw::Sound* _pSound;
	bool _isPlayAnyPoint;
};

class VoiceSegsQueue{
public:
	void add(VoiceSegs* pSegs);
	void main();
	void setVolume(float volume);

private:
	std::list<VoiceSegs*> _queue;
	float _nextPoint;
};

#endif //__VOICE_H__