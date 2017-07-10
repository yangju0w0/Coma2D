/*
* filename	Event.h
* fileinfo	이벤트 정보 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

/*===================================================================================
Event.h
Event클래스는 이벤트의 정보를 담는 클래스이다. EventDispatcher의 dispatchEvent에서 인
자로 사용된다. 이 경우, 등록된 리스너의 함수에 인자로 전달된다. 즉, 발생한 이벤트에
대한 정보를 해당 함수에 알릴 수 있다는 것이다.

이벤트 발생 순서
dispatchEvent(new Event("mouseEvent"))
=> type이 "mouseEvent"인 리스너 탐색
=> 존재할 경우 해당 함수 실행 (Event* 인자 전달)

이벤트 등록 순서
void mouseListener(Event* event);
EventDispatcher->setEventListener("mouseEvent", BIND(mouseListener));
=> 이벤트 발생 시 mouseListener함수 호출


static const std::string을 이용하여 이벤트 타입을 지정할 수 있다.
또한 상속을 받아, 이벤트와 관련된 데이터를 전달할 수도 있다.

예) MouseEvent (Event클래스 상속)
dispatchEvent(new MouseEvent( MouseEvent::CLICK, x, y ));
setEventListener(MouseEvent::CLICK, BIND(mouseListener));
void mouseListener(Event* event)
{
	MouseEvent* mouseEvent = (MouseEvent*) event;
	mouseEvent->getX();
	mouseEvent->getY();
}
====================================================================================*/

#pragma once
#include <string>
#include "comaUtil.h"

COMA_NS_BEGIN

class Event
{
public:
	explicit Event(const std::string& type);
	virtual ~Event();
	const std::string& getType() const
	{
		return type;
	}
private:
	const std::string type;
};

COMA_END