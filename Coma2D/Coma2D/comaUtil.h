/*
* filename	comaUtil.h
* fileinfo	매크로 및 유틸리티 집합 파일
* author	주헌양 (Heonyang Ju)
*/
#include <d2d1.h>
#include <math.h>
#include "Box2D/Box2D.h"

#define COMA_NAMESPACE coma2d
#define COMA_NS_BEGIN namespace COMA_NAMESPACE {
#define COMA_USING_NS using namespace COMA_NAMESPACE;
#define COMA_END }

#define PI 3.1415926535f
#define RADIAN_TO_DEGREE(x)((x)*(180.0f/PI));
#define DEGREE_TO_RADIAN(x)((x)*(PI/180.0f));

typedef D2D_POINT_2U PointU;
typedef D2D_POINT_2F Point;
typedef D2D_RECT_F Rect;
typedef D2D_RECT_U RectU;
typedef D2D_SIZE_F Size;
typedef D2D_SIZE_U SizeU;
typedef D2D_COLOR_F Color;
typedef D2D1::Matrix3x2F Matrix3x2;
typedef UINT64 TAG;
typedef b2Vec2 Vector;