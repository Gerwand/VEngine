#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"


#include "Frustum.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "Managers/GlPipelineManager.h"
#include "BoundingBox.h"

namespace vengine {

class CameraFPP {
public:
	CameraFPP(float fov = 70.0f, bool perspective = true);

	void SetFOV(float fov);

	void Move(const Vector3& vector);
	void SetPosition(const Vector3& position);
	
	void RotateHorizontal(float angle);
	void RotateVertical(float angle);
	void Rotate(float horizontalAngle, float verticalAngle);
	void SetRotation(float horizontalAngle, float verticalAngle);
	void ChangePerspective(bool perspective);
	const Vector3& GetDirection();
	float GetVertical();
	float GetHorizontal();
	const Vector3& GetPosition() const;

	void GetProjectionMatrix(Matrix4* proj, int w, int h);
	const Matrix4& GetViewMatrix();

	bool OrientationChanged();
	bool PerspectiveChanged();

	bool IsVisibleSmall(const BoundingBox& boundary);
	bool IsVisibleBig(const BoundingBox& boundary);

private:
	Vector3 _position;
	Vector2 _orientation;
	Vector3 _viewDirection;
	
	bool _perspective;
	bool _moved;
	bool _perspectiveChanged;
	bool _orientationChanged;
	float _sensitivity = 0.25;

	Frustum _viewFrustum;

	Matrix4 _viewMatrix;
	Matrix4 _projectionMatrix;
};

inline const Vector3&
CameraFPP::GetPosition() const
{
	return _position;
}

inline void 
CameraFPP::ChangePerspective(bool perspective)
{
	_perspectiveChanged = perspective != _perspective;
	_perspective = perspective;
}

}