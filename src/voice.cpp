#include "stdafx.h"
#include "voice.h"

VoiceSegs::VoiceSegs(const char* file, const float* firstSegPoint, int size, bool isPlayAnyPoint)
:_firstSegPoint(firstSegPoint), _size(size), _isPlayAnyPoint(isPlayAnyPoint){
	_pSound = lw::SoundMgr::s().createSound(file);
	_pSound->setLoop(true);
	lwassert(_pSound);
}

VoiceSegs::~VoiceSegs(){
	delete _pSound;
}

void VoiceSegs::play(){
	if ( _isPlayAnyPoint ){
		playAnyPoint();
	}else{
		_pSound->setSecOffset(0.f);
		_pSound->play();
	}
}

void VoiceSegs::playAnyPoint(){
	int rd = rand()%_size;
	float secOffset = 0.f;
	if ( rd != _size-1 ){
		secOffset = _firstSegPoint[rd];
	}
	_pSound->setSecOffset(secOffset);
	_pSound->play();
}

float VoiceSegs::getNextPoint(){
	float secOffset = _pSound->getSecOffset();
	float nextPoint = 0.f;
	for ( int i = 0; i < _size-1; ++i ){
		if ( secOffset < _firstSegPoint[i] ){
			nextPoint = _firstSegPoint[i];
			break;
		}
	}
	return nextPoint;
}

void VoiceSegsQueue::add(VoiceSegs* pSegs){
	if ( _queue.empty() ){
		_queue.push_back(pSegs);
		pSegs->play();
    }else{
		if ( _queue.size() == 2 ){
            if ( _queue.back() == pSegs ){
                return;
            }
			_queue.pop_back();
		}
		if ( pSegs != _queue.front() ){
			_queue.push_back(pSegs);
			_nextPoint = _queue.front()->getNextPoint();
		}
	}
    
}

void VoiceSegsQueue::main(){
	if ( _queue.size() == 2 ){
		lw::Sound* pSound = _queue.front()->getSound();
		float secOffset = pSound->getSecOffset();
		if ( _nextPoint == 0.f ){
			if ( secOffset < .1f ){
				pSound->stop();
				_queue.pop_front();
				_queue.front()->play();
			}
		}else{
			if ( secOffset >= _nextPoint ){
				pSound->stop();
				_queue.pop_front();
				_queue.front()->play();
			}
		}
	}
}

void VoiceSegsQueue::setVolume(float volume){
	if ( !_queue.empty() ){
		_queue.front()->getSound()->setVolumeNow(volume);
	}
}