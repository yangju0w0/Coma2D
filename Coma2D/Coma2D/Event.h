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
	Event(std::string type);	//생성자를 통해 이벤트 타입을 지정한다.
	virtual ~Event();
	std::string getType()	//이벤트 타입 Getter
	{
		return type;
	}
private:
	std::string type;	//이벤트 타입을 저장한다.
};

COMA_END