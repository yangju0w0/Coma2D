/*
* The MIT License (MIT)
*
* Copyright (c) 2015 Heonyang Ju
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*
* MIT 라이선스 (MIT)
*
* 저작권(C) 2015 주헌양
*
* 이 문서에 의해, 아래의 조건을 따르는 한 제한 없이 소프트웨어를 다루기 위해
* 소프트웨어의 복사본과 관련된 문서("소프트웨어")를 획득하는 사람에게 변경의
* 자유가 허용 된다. 제한 없음은 사용, 복제, 수정, 통합, 공포, 배포, 하위 라이선스
* 그리고/또는 소프트웨어의 복사본의 판매와 공급된 소프트웨어를 가진 사람에게
* 그렇게 할 수 있도록 허가하는 권한에 제한 없음을 포함 한다.
*
* 위의 저작권 표시와 이 허가 표시는 모든 소프트웨어의 복사본 또는 상당한 일부분에
* 포함되어야 한다.
*
* 이 소프트웨어는 특정한 목적과 침해 없음을 위해 상업성, 적합성의 보증에만
* 국한되는 것이 아닌 명시적 또는 묵시적인 어떠한 종류의 보증없이 "있는 그대로"
* 제공 된다. 어떠한 경우에도 저작자 또는 저작권자는 소프트웨어의 관계 안과 밖
* 또는 소프트웨어의 사용 또는 거래로부터 발생하는 계약, 불법 행위 또는 기타 수행에
* 대해, 어떠한 소송, 손해 또는 다른 책무에 대해서도 책임이 없다.
*
* 본문은 MIT라이선스 원문의 번역본이며, 모든 해석은 원문에 기초한다.
*/

/*
* filename	EventDispatcher.h
* fileinfo	이벤트 분배 및 처리 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/


/*===================================================================================
EventDispatcher.h
EventDispatcher클래스는 이벤트를 전달하고 등록, 해제, 발생을 담당하는 기본 클래스이다.
대부분의 Coma2D 클래스가 EventDispatcher을 상속받으며, 이벤트를 전달하기 위해선 이 클
래스를 상속받아야 한다.

void setEventListener(string, EventFunction)
이벤트 리스너를 리스트에 추가하는 역할을 한다. 등록시에는 void(Event*)형식의 함수를
bind하고, 이벤트를 구분짓는 타입(string)을 지정해야 한다.
일반적으로 type은 해당 타입을 지원하는 Event클래스에서 static const std::string의
형태로 제공된다.

void removeEventListener(string, EventFunction)
이벤트 리스너를 리스트에서 제거하는 역할을 한다.

bool hasEventListener(string);
해당 이벤트 타입이 리스너에 존재할 경우 true를 반환한다.

void dispatchEvent(Event*)
이벤트를 발생시킨다. 해당 이벤트와 동일한 타입을 가진 리스너를 모두 실행시킨다. 함수를
실행시 Event객체를 인자로 전달한다.
====================================================================================*/

#pragma once
#include <functional>
#include <vector>
#include <string>
#include "Event.h"
#define BIND(T) std::bind(&T, this, std::placeholders::_1)

COMA_NS_BEGIN

typedef std::function<void(Event*)> EventFunction;

struct Listener	//이벤트 리스너를 저장하는 구조체
{
	std::string type;		//이벤트 타입
	EventFunction function;	//실행 함수
	void* target;			//이벤트를 선언한 클래스
};


class EventDispatcher
{
public:
	EventDispatcher();
	virtual ~EventDispatcher();
	
	void setEventListener(std::string type, EventFunction function, void* target = nullptr);			//이벤트 리스너 객체를 등록한다.
	void removeEventListener(std::string type, void* target);		//이벤트 리스너 객체를 제거한다.
	bool hasEventListener(std::string type);								//해당 타입의 리스너가 존재할 경우 true반환
	void dispatchEvent(Event* event);										//이벤트를 발생시킨다 (등록된 해당 리스너 실행)
private:
	std::vector<Listener> listenerList;	//이벤트 리스너가 저장된 리스트
};

COMA_END