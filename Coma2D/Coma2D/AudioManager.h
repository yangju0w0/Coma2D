/*
* filename	AudioManager.h
* fileinfo	오디오 관리 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Manager.h"

COMA_NS_BEGIN

class AudioManager :
	public Manager
{
public:
	AudioManager();
	virtual ~AudioManager();
};

COMA_END