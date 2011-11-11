#include "stdafx.h"
#include "clock.h"

void Clock::toString(std::wstring& outStr, int time){
	std::wstringstream ss;

	//minute
	int m = time/60000;
	int digi = 0;

	if ( m >= 10 ){
		digi = m/10;
		ss << digi;
	}
	digi = m / 10;
	ss << digi;
	digi = m % 10;
	ss << digi;
	ss << ":";

	//second
	int s = (time - m * 60000)/1000;
	digi = s/10;
	ss << digi;
	digi = s % 10;
	ss << digi;
	ss << ":";

	//ms
	int ms = time - m*60000 - s*1000;
	digi = ms/100;
	ss << digi;
	digi = (ms % 100)/10;
	ss << digi;

	outStr = ss.str(); 
}