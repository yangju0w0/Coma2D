/*
* filename	DisplayObject.h
* fileinfo	화면 표시 객체 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "EventDispatcher.h"
#include <math.h>
#include <d2d1.h>

COMA_NS_BEGIN

class DisplayObjectContainer;
class Camera;

class DisplayObject :
	public EventDispatcher
{
protected:
	DisplayObject();
	void SetLocalSize(const Size& size);
	void SetLocalSize(float width, float height);
	void SetLocalPosition(const Point& position);
	void SetLocalPosition(float x, float y);

	static ID2D1SolidColorBrush* brush;
private:
	static DisplayObjectContainer* world;

	DisplayObjectContainer* parentObject_;
	Camera* camera_;

	Point position_;
	Size scale_;
	float rotation_;
	Point anchorPoint_;
	bool visible_;
	float alpha_;
	Size localSize_;
	Point localPoint_;
	bool drawOutline_;

public:
	virtual ~DisplayObject();

	virtual void Update(double deltaTime); // 렌더링 이전에 호출
	virtual void Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime); // 렌더링

//Getter
	static DisplayObjectContainer* GetWorld();
	Camera* GetCamera() const;

	Point GetPosition() const;
	float GetX() const;
	float GetY() const;
	Size GetSize() const;
	float GetWidth() const;
	float GetHeight() const;
	const Size& GetLocalSize() const;
	const Point& GetLocalPosition() const;
	const Size& GetScale() const;
	float GetScaleX() const;
	float GetScaleY() const;
	float GetRotation() const;
	float GetRotationRad() const;
	const Point& GetAnchorPoint() const;
	bool IsVisible() const;
	float GetAlpha() const;
	float GetScreenAlpha() const;
	bool IsOutlineDrawing() const;

	Matrix3x2 GetScaleMatrix() const; //화면 비율 행렬을 반환한다.
	Matrix3x2 GetTranslationMatrix() const; //이동 행렬을 반환한다.
	Matrix3x2 GetRotationMatrix() const; //회전 행렬을 반환한다.
	Matrix3x2 GetCameraMatrix() const; //카메라의 변환 행렬을 반환한다.
	Matrix3x2 GetMatrix() const; //해당 Object의 변환 행렬을 반환한다.
	Matrix3x2 GetWorldMatrix() const; //월드 오브젝트 내에서의 행렬 변환을 반환한다. 주의: 월드에 포함되지 않은 경우 정확한 결과가 나오지 않을 수 있음.
	Matrix3x2 GetScreenMatrix() const; //해당 오브젝트를 포함한 상위 오브젝트의 행렬의 곱을 반환한다. (재귀)

	DisplayObjectContainer* GetParent() const; //부모 객체를 반환한다.
	virtual bool _RegisterParent(DisplayObjectContainer* parent); //부모 객체를 등록한다. (외부호출 금지)
	virtual bool _UnregisterParent(); //부모 객체를 해제한다. (외부호출 금지)

//Setter
	static void _SetWorld(DisplayObjectContainer* world);
	void SetCamera(Camera* camera);	//카메라 설정
	void UnsetCamera();	//카메라 해제

	void SetPosition(float x, float y);
	void SetPosition(Point position);
	void SetX(float value);
	void SetY(float value);
	void Translate(Point vector);
	void Translate(float x, float y);
	void TranslateX(float value);
	void TranslateY(float value);
	void SetSize(float width, float height);
	void SetSize(Size size);
	void SetWidth(float value);
	void SetHeight(float value);
	void SetScale(float scaleX, float scaleY);
	void SetScale(Size scale);
	void SetScaleX(float value);
	void SetScaleY(float value);
	void SetRotation(float rotation);
	void SetRotationRad(float rotation);
	void SetAnchorPoint(float x, float y);
	void SetAnchorPoint(Point point);
	void SetAlpha(float alpha);
	void SetVisible(bool visibility);
	virtual void SetDrawOutline(bool draw);

	//World좌표계를 기준으로 위치를 변환한다.
	Point WorldToLocal(Point point) const;
	Point LocalToWorld(Point point) const;
	//윈도우 화면의 좌표계를 기준으로 위치를 변환한다.
	Point ScreenToLocal(Point point) const;
	Point LocalToScreen(Point point) const;

	virtual void _TransformApply();	//변환을 부모 객체에게 알릴 때 호출한다.(외부호출 금지)

protected:
	virtual void DrawOutline(ID2D1HwndRenderTarget* renderTarget);
};

COMA_END