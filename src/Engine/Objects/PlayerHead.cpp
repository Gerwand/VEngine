#include "PlayerHead.h"

namespace vengine {
void
PlayerHead::OnUpdate()
{
	if (Input::GetCursorMode())
		return;

	Vector2 rot = Input::GetMouseOffset();
	_camera->Rotate(rot.x, -rot.y);

	/* Rotate head */
	Quaternion quat1, quat2;
	quat1.SetRotation(-_camera->GetVertical(), Vector3::forward);
	quat2.SetRotation(-_camera->GetHorizontal(), Vector3::up);
	_transform.SetRotation(quat2*quat1);
}

void
PlayerHead::OnLateUpdate()
{
	_camera->SetPosition(_transform.GetWorldPosition());
}


}