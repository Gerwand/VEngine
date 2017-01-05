#pragma once

#include "VEMath.h"
#include "Math/Plane.h"

namespace vengine {

class Frustum
{
public:
	enum PlaneIndex {
		PLANE_NEAR,
		PLANE_FAR,
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_TOP,
		PLANE_BOTTOM,
		NUM_OF_PLANES
	};

	Frustum(float fov, float aspect, float nearDist, float farDist);

	void Set(const Matrix4& mat);
	void SetCulling(float fov, float aspect, float nearDist, float farDist);
	void SetPosition(const Vector3& pos, const Vector3& front, const Vector3& up, const Vector3& right);

	void SetFov(float fov);
	void SetAspect(float aspect);
	void SetClipping(float nearDist, float farDist);

	const Plane& GetPlane(int index) const;
	float GetNear() const;
	float GetFar() const;
	float GetFov() const;
	float GetAspect() const;

	void GetFarPoints(const Vector3** topLeft, const Vector3** topRight, const Vector3** botLeft, const Vector3** botRight);
	void GetNearPoints(const Vector3** topLeft, const Vector3** topRight, const Vector3** botLeft, const Vector3** botRight);

	const Vector3& GetNearCenter() const;
	const Vector3& GetFarCenter() const;
private:
	float _near;
	float _far;
	float _aspect;
	float _fov;
	Vector3 _nearCenter, _farCenter;
	Vector3 _pos, _front, _up, _right;
	Vector3 _farTopLeft, _farTopRight, _farBottomLeft, _farBottomRight;
	Vector3 _nearTopLeft, _nearTopRight, _nearBottomLeft, _nearBottomRight;

	/* Info from culling info, do not need to be recalculated everytime */
	float _nearW;
	float _nearH;
	float _farW;
	float _farH;

	union {
		struct {
			Plane _pNear;
			Plane _pFar;
			Plane _pLeft;
			Plane _pRight;
			Plane _pTop;
			Plane _pBottom;
		};
		Plane _planes[6];
	};

	void Recalculate();
};


inline void
Frustum::SetFov(float fov)
{
	_fov = fov;

	float tang = tan(degToRad(_fov));
	_nearH = _near * tang;
	_nearW = _nearH * _aspect;
	_farW = _far  * tang;
	_farH = _farW * _aspect;

	//if (_front != Vector3::zeroes)
		//Recalculate();
}
inline void
Frustum::SetAspect(float aspect)
{
	_aspect = aspect;

	float tang = tan(degToRad(_fov));
	_nearH = _near * tang;
	_nearW = _nearH * _aspect;
	_farW = _far  * tang;
	_farH = _farW * _aspect;

	//if (_front != Vector3::zeroes)
		//Recalculate();
}

inline void 
Frustum::SetClipping(float nearDist, float farDist)
{
	_near = nearDist;
	_far = farDist;

	float tang = tan(degToRad(_fov));
	_nearH = _near * tang;
	_nearW = _nearH * _aspect;
	_farW = _far  * tang;
	_farH = _farW * _aspect;

	//if (_front != Vector3::zeroes)
		//Recalculate();
}

inline const Plane& 
Frustum::GetPlane(int index) const
{
	assert(index < NUM_OF_PLANES, "Index out of bound: %d", index);
	return _planes[index];
}

inline float 
Frustum::GetNear() const
{
	return _near;
}

inline float 
Frustum::GetFar() const
{
	return _far;
}

inline float 
Frustum::GetFov() const
{
	return _fov;
}

inline float 
Frustum::GetAspect() const
{
	return _aspect;
}

inline const Vector3&
Frustum::GetNearCenter() const
{
	return _nearCenter;
}

inline const Vector3&
Frustum::GetFarCenter() const
{
	return _farCenter;
}

inline void 
Frustum::GetFarPoints(const Vector3** topLeft, const Vector3** topRight, const Vector3** botLeft, const Vector3** botRight)
{
	*topLeft = &_farTopLeft;
	*topRight = &_farTopRight;
	*botLeft = &_farBottomLeft;
	*botRight = &_farBottomRight;
}
inline void 
Frustum::GetNearPoints(const Vector3** topLeft, const Vector3** topRight, const Vector3** botLeft, const Vector3** botRight)
{
	*topLeft = &_nearTopLeft;
	*topRight = &_nearTopRight;
	*botLeft = &_nearBottomLeft;
	*botRight = &_nearBottomRight;
}

}