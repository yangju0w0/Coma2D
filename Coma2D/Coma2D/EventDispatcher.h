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