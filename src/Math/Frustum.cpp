#include "Frustum.h"


namespace vengine {

Frustum::Frustum(float fov, float aspect, float nearDist, float farDist)
{
	_fov = fov;
	_aspect = aspect;
	_near = nearDist;
	_far = farDist;
}

void 
Frustum::Set(const Matrix4& mat)
{
	_pNear.Set(Vector3(
		mat[0][2] + mat[0][3],
		mat[1][2] + mat[1][3],
		mat[2][2] + mat[2][3]),
		mat[3][2] + mat[3][3]);
	_pFar.Set(Vector3(
		-mat[0][2] + mat[0][3],
		-mat[1][2] + mat[1][3],
		-mat[2][2] + mat[2][3]),
		-mat[3][2] + mat[3][3]);
	_pBottom.Set(Vector3(
		mat[0][1] + mat[0][3],
		mat[1][1] + mat[1][3],
		mat[2][1] + mat[2][3]),
		mat[3][1] + mat[3][3]);
	_pTop.Set(Vector3(
		-mat[0][1] + mat[0][3],
		-mat[1][1] + mat[1][3],
		-mat[2][1] + mat[2][3]),
		-mat[3][1] + mat[3][3]);
	_pLeft.Set(Vector3(
		mat[0][0] + mat[0][3],
		mat[1][0] + mat[1][3],
		mat[2][0] + mat[2][3]),
		mat[3][0] + mat[3][3]);
	_pRight.Set(Vector3(
		-mat[0][0] + mat[0][3],
		-mat[1][0] + mat[1][3],
		-mat[2][0] + mat[2][3]),
		-mat[3][0] + mat[3][3]);
}

void
Frustum::Recalculate()
{
	/*
	_nearCenter = _pos + _front * _near;
	_farCenter = _pos + _front * _far;


	Vector3 nearUp = _up * _nearH / 2.0f;
	Vector3 nearRight = _right * _nearW / 2.0f;

	Vector3 farUp = _up * _farH / 2.0f;
	Vector3 farRight = _right * _farW / 2.0f;

	_nearTopLeft = _nearCenter + nearUp - nearRight;
	_nearTopRight = _nearCenter + nearUp + nearRight;
	_nearBottomLeft = _nearCenter - nearUp - nearRight;
	_nearBottomRight = _nearCenter - nearUp + nearRight;

	_farTopLeft = _farCenter + farUp - farRight;
	_farTopRight = _farCenter + farUp + farRight;
	_farBottomLeft = _farCenter - farUp - farRight;
	_farBottomRight = _farCenter - farUp + farRight;

	_pNear.Set(_front, _nearCenter);
	_pFar.Set(-_front, _farCenter);


	_pLeft.Set(_nearBottomLeft, _farBottomLeft, _farTopLeft);
	_pRight.Set(_nearBottomRight, _farTopRight, _farBottomRight);

	_pTop.Set(_nearTopLeft, _farTopLeft, _farTopRight);
	_pBottom.Set(_nearBottomRight, _farBottomRight, _farBottomLeft);
	*/
}

void 
Frustum::SetCulling(float fov, float aspect, float nearDist, float farDist)
{
	_fov = fov;
	_aspect = aspect;
	_near = nearDist;
	_far = farDist;
	/*
	float tang = tan(degToRad(_fov));
	_nearH = _near * tang;
	_nearW = _nearH * _aspect;
	_farW = _far  * tang;
	_farH = _farW * _aspect;

	if (_front != Vector3::zeroes)
		Recalculate();
	*/
}

void
Frustum::SetPosition(const Vector3& pos, const Vector3& front, const Vector3& up, const Vector3& right)
{
	_pos = pos;
	_front = front;
	_up = up;
	_right = right;
	
	//Recalculate();
}



}
