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
* MIT ���̼��� (MIT)
*
* ���۱�(C) 2015 �����
*
* �� ������ ����, �Ʒ��� ������ ������ �� ���� ���� ����Ʈ��� �ٷ�� ����
* ����Ʈ������ ���纻�� ���õ� ����("����Ʈ����")�� ȹ���ϴ� ������� ������
* ������ ��� �ȴ�. ���� ������ ���, ����, ����, ����, ����, ����, ���� ���̼���
* �׸���/�Ǵ� ����Ʈ������ ���纻�� �Ǹſ� ���޵� ����Ʈ��� ���� �������
* �׷��� �� �� �ֵ��� �㰡�ϴ� ���ѿ� ���� ������ ���� �Ѵ�.
*
* ���� ���۱� ǥ�ÿ� �� �㰡 ǥ�ô� ��� ����Ʈ������ ���纻 �Ǵ� ����� �Ϻκп�
* ���ԵǾ�� �Ѵ�.
*
* �� ����Ʈ����� Ư���� ������ ħ�� ������ ���� �����, ���ռ��� ��������
* ���ѵǴ� ���� �ƴ� ������ �Ǵ� �������� ��� ������ �������� "�ִ� �״��"
* ���� �ȴ�. ��� ��쿡�� ������ �Ǵ� ���۱��ڴ� ����Ʈ������ ���� �Ȱ� ��
* �Ǵ� ����Ʈ������ ��� �Ǵ� �ŷ��κ��� �߻��ϴ� ���, �ҹ� ���� �Ǵ� ��Ÿ ���࿡
* ����, ��� �Ҽ�, ���� �Ǵ� �ٸ� å���� ���ؼ��� å���� ����.
*
* ������ MIT���̼��� ������ �������̸�, ��� �ؼ��� ������ �����Ѵ�.
*/

/*
* filename	DisplayObject.cpp
* fileinfo	ȭ�� ǥ�� ��ü Ŭ���� ���� ����
* author	����� (Heonyang Ju)
*/

#include "DisplayObject.h"
#include "DisplayObjectContainer.h"
#include "Camera.h"
COMA_USING_NS

DisplayObjectContainer* DisplayObject::world = nullptr;

DisplayObject::DisplayObject()
:position(Point{ 0, 0 }), scale(Size{ 1, 1 }), rotation(0), anchorPoint(Point{ 0, 0 }), visible(true), alpha(1.0f), localSize(Size{ 0, 0 })
{
}


DisplayObject::~DisplayObject()
{
	if (getParent())
	{ 
		getParent()->removeChild(this);
	}
}

Matrix3x2 DisplayObject::getCameraMatrix()
{
	if (camera)
	{
		Matrix3x2 inverted = camera->getMatrix();
		inverted.Invert();
		return inverted;
	}
	return Matrix3x2::Identity();
}

void DisplayObject::setCamera(Camera* camera)
{
	if (camera)
	{
		this->camera = camera;
		camera->_registerParent(parentObject);
	}
}
void DisplayObject::unsetCamera()
{
	if (this->camera)
	{
		camera->_unregisterParent();
		camera = nullptr;
	}
}
void DisplayObject::_setWorld(DisplayObjectContainer* world)
{
	DisplayObject::world = world;
}

Matrix3x2 DisplayObject::getScreenMatrix()
{
	if (getParent())
		return getMatrix() * getCameraMatrix()* getParent()->getScreenMatrix();
	return getCameraMatrix() * getMatrix();
}
Matrix3x2 DisplayObject::getWorldMatrix()
{
	Matrix3x2 invertedMatrix = world->getMatrix() * world->getCameraMatrix();
	invertedMatrix.Invert();
	return getScreenMatrix()* invertedMatrix;
}

float DisplayObject::getScreenAlpha()
{
	if (getParent())
		return getAlpha() * getParent()->getScreenAlpha();
	return getAlpha();
}

void DisplayObject::_transformApply()
{
	if (getParent())
	{
		getParent()->_resetSize();
		getParent()->_transformApply();
	}
}