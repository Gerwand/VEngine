#pragma once

#include "VEMath.h"
#include "Resources/Managers/GlPipelineManager.h"
#include "Physic/BoundingBox.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace vengine {

/*
* Object representing advanced camera with First Person Perspective transformations.
*/
class CameraFPP {
public:
	/* Construct new camera, default FOV will be 70 degrees and perspective. */
	CameraFPP(float fov = 70.0f, bool perspective = true);

	/* Set Field Of View. Should be less than 90 */
	void SetFOV(float fov);

	/* Move camera object by given vector */
	void Move(const Vector3& vector);
	/* Set position of the camera */
	void SetPosition(const Vector3& position);
	
	/* Rotate camera horizontaly by given angle */
	void RotateHorizontal(float angle);
	/*
	* Rotate camera verticaly by given angle - cannot exceed 89.9 degrees to prevent up vector flipping.
	* It could be solved by using quaternions, but euler angles as a camera are much simpler and faster
	*/
	void RotateVertical(float angle);
	/* Rotate camera by given angles */
	void Rotate(float horizontalAngle, float verticalAngle);
	/* Set rotations of the camera */
	void SetRotation(float horizontalAngle, float verticalAngle);

	/* 
	* Change projection of the camera
	* @param perspective if this value is true, then camera will use perspective projection, else it will use orthogonal projection 
	*/
	void ChangePerspective(bool perspective);
	
	/* Get normalized vector which is pointing in the same direction where the camera is looking */
	const Vector3& GetDirection() const;
	/* Get vertical rotation of the camera */
	float GetVertical() const;
	/* Get horizontal rotation of the camera */
	float GetHorizontal() const;
	/* Get position of the camera */
	const Vector3& GetPosition() const;

	/* Get projection matrix using given width and height of the projection */
	void GetProjectionMatrix(Matrix4* proj, int w, int h);
	/* Get view matrix of the camera */
	const Matrix4& GetViewMatrix();

	/* Check if orientation changed since last time it was checked */
	bool OrientationChanged();
	/* Check if perspective changed since last time it was checked */
	bool PerspectiveChanged();

	/* Check if given AABB is inside camera's frustum */
	bool IsVisible(const BoundingBox& boundary);

private:
	Vector3 _position;		/* World's position of the camera */
	Vector2 _orientation;	/* Orientation of the camera (horizontal and vertical) */
	Vector3 _viewDirection; /* Direction where camera is pointing */
	
	bool _perspective;			/* Flag indicating that projection is perspective if true, else it is orthogonal */
	bool _moved;				/* Flag indicating that camera was moved since last calculating view matrix */
	bool _perspectiveChanged;	/* Flag indicating that perspective changed since last check */
	bool _orientationChanged;	/* Flag indicating that orientation changed since last check */
	const float _sensitivity = 0.25;	/* Sensitivity of the mouse rotation */

	Frustum _viewFrustum;		/* Frustum representing area seen by the camera */

	Matrix4 _viewMatrix;		/* View matrix of the camera */
	Matrix4 _projectionMatrix;	/* Projection matrix of the camera */
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