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
	/* For near we have 0 instead of 1, so we can skip w_c from matrix*/
	_pNear.Set(Vector3(
		mat[0][2],
		mat[1][2],
		mat[2][2]),
		mat[3][2]);
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
Frustum::SetCulling(float fov, float aspect, float nearDist, float farDist)
{
	_fov = fov;
	_aspect = aspect;
	_near = nearDist;
	_far = farDist;
}

}
