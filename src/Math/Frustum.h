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

	void SetFov(float fov);
	void SetAspect(float aspect);
	void SetClipping(float nearDist, float farDist);

	const Plane& GetPlane(int index) const;
	float GetNear() const;
	float GetFar() const;
	float GetFov() const;
	float GetAspect() const;
private:
	float _near;
	float _far;
	float _aspect;
	float _fov;

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

};


inline void
Frustum::SetFov(float fov)
{
	_fov = fov;
}
inline void
Frustum::SetAspect(float aspect)
{
	_aspect = aspect;
}

inline void 
Frustum::SetClipping(float nearDist, float farDist)
{
	_near = nearDist;
	_far = farDist;
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

}