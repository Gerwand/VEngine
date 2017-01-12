#include "CameraFPP.h"

namespace vengine {

CameraFPP::CameraFPP(float fov, bool perspective) : _position(0.0f), _orientation(-90.0f, 0.0f),
	_viewFrustum(fov, 1.0f, 0.1f, 1000.0f)
{
	assert(fov < 90.0f, "FOV cannot be over 90 degrees");
	_moved = true;
	_perspective = perspective;
	_perspectiveChanged = true;
	_orientationChanged = true;
}

void
CameraFPP::SetFOV(float fov)
{
	assert(fov < 90.0f, "FOV cannot be over 90 degrees");
	_viewFrustum.SetFov(fov);
	_perspectiveChanged = true;
}

void
CameraFPP::Move(const Vector3& vector)
{
	if (vector == Vector3::zeroes)
		return;

	_position += vector;
	_moved = true;
	_orientationChanged = true;
}
void
CameraFPP::SetPosition(const Vector3& position)
{
	_position = position;
	_moved = true;
	_orientationChanged = true;
}

void
CameraFPP::RotateHorizontal(float angle) 
{
	if (angle == 0.0f)
		return;

	_orientation.x += angle * _sensitivity;

	if (_orientation.x >= 360.0f)
		_orientation.x -= 360.0f;
	else if (_orientation.x <= -360.0f)
		_orientation.x += 360.0f;

	_moved = true;
	_orientationChanged = true;
}
void
CameraFPP::RotateVertical(float angle)
{
	if (angle == 0.0f)
		return;

	_orientation.y += angle * _sensitivity * _viewFrustum.GetAspect();

	if (_orientation.y >= 89.9f)
		_orientation.y = 89.9f;
	else if (_orientation.y <= -89.9f)
		_orientation.y = -89.9f;

	_moved = true;
	_orientationChanged = true;
}

float
CameraFPP::GetHorizontal() const
{
	return _orientation.x;
}

float
CameraFPP::GetVertical() const
{
	return _orientation.y;
}

void
CameraFPP::Rotate(float horizontalAngle, float verticalAngle)
{
	RotateHorizontal(horizontalAngle);
	RotateVertical(verticalAngle);
}

void
CameraFPP::SetRotation(float horizontalAngle, float verticalAngle)
{
	_orientation.Set(horizontalAngle, verticalAngle);
	_moved = true;
	_orientationChanged = true;
}

void
CameraFPP::GetProjectionMatrix(Matrix4* proj, int w, int h)
{
	_viewFrustum.SetAspect(w / (float)h);
	
	if (_perspective) {
		*proj = Matrix4::GetPerspective(_viewFrustum.GetFov(), _viewFrustum.GetAspect(),
										_viewFrustum.GetNear(), _viewFrustum.GetFar());
		_projectionMatrix = *proj;
	}
	else {
		/* Used just for debuging and testing, it is size of the world. */
		*proj = Matrix4::GetOrtho(-128 * _viewFrustum.GetAspect(), 128 * _viewFrustum.GetAspect(),
								  -128, 128, _viewFrustum.GetNear(), _viewFrustum.GetFar());
		_projectionMatrix = *proj;
	}
}

const Matrix4&
CameraFPP::GetViewMatrix()
{
	/* Update view only if moved */
	if (_moved) {
		/* Use simple euler angles math S*/
		Vector3 front;
		front.x = cos(degToRad(_orientation.x)) * cos(degToRad(_orientation.y));
		front.y = sin(degToRad(_orientation.y));
		front.z = sin(degToRad(_orientation.x)) * cos(degToRad(_orientation.y));
		front.Normalize();

		Vector3 right = Vector3::Cross(front, Vector3::up);
		right.Normalize();
		Vector3 up = Vector3::Cross(right, front);
		up.Normalize();
		_viewDirection = front;
		_viewMatrix = Matrix4::GetLookAt(_position, _position + front, up);
		_moved = false;

		/* Update frustum */
		_viewFrustum.Set(_projectionMatrix * _viewMatrix);
	}
	return _viewMatrix;
}

bool
CameraFPP::OrientationChanged()
{
	if (_orientationChanged) {
		_orientationChanged = false;
		return true;
	}
	else {
		return false;
	}

}
bool
CameraFPP::PerspectiveChanged()
{
	if (_perspectiveChanged) {
		_perspectiveChanged = false;
		return true;
	}
	else {
		return false;
	}
}

bool
CameraFPP::IsVisible(const BoundingBox& boundary)
{
	/* Check if objet is visible using p vertices */
	for (int i = 0; i < Frustum::NUM_OF_PLANES; i++) {
		const Plane& plane = _viewFrustum.GetPlane(i);

		Vector3 pVert;
		boundary.GetPVertex(&pVert, plane.GetNormal());

		/* If any pvertex is not inside, it is not visible */
		if (plane.Distance(pVert) < 0.0f)
			return false;
	}

	return true;
}


const Vector3& 
CameraFPP::GetDirection() const
{
	return _viewDirection;
}

}