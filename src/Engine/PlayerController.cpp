#include "PlayerController.h"

namespace vengine {

void
PlayerController::OnDraw(Renderer* renderer)
{
	PhysicalObject::OnDraw(renderer);

	if (rayInfo.CollisionFound()) {
		Chunk* hitCh = rayInfo.GetCollidedChunk();
		const Vector3& coord = rayInfo.GetVoxelCoordinates();
		_voxLines.ClearVertices();
		_voxLines.SetColor(Vector3(0.1f, 0.1f, 0.1f));
		_voxLines.AddLine(Vector3(coord.x - 0.001f, coord.y - 0.001f, coord.z - 0.001f),
						  Vector3(coord.x + 1.001f, coord.y - 0.001f, coord.z - 0.001f));
		_voxLines.AddLine(Vector3(coord.x - 0.001f, coord.y - 0.001f, coord.z - 0.001f),
						  Vector3(coord.x - 0.001f, coord.y + 1.001f, coord.z - 0.001f));
		_voxLines.AddLine(Vector3(coord.x - 0.001f, coord.y - 0.001f, coord.z - 0.001f),
						  Vector3(coord.x - 0.001f, coord.y - 0.001f, coord.z + 1.001f));

		_voxLines.AddLine(Vector3(coord.x + 1.001f, coord.y - 0.001f, coord.z + 1.001f),
						  Vector3(coord.x - 0.001f, coord.y - 0.001f, coord.z + 1.001f));
		_voxLines.AddLine(Vector3(coord.x + 1.001f, coord.y - 0.001f, coord.z + 1.001f),
						  Vector3(coord.x + 1.001f, coord.y - 0.001f, coord.z - 0.001f));
		_voxLines.AddLine(Vector3(coord.x + 1.001f, coord.y - 0.001f, coord.z + 1.001f),
						  Vector3(coord.x + 1.001f, coord.y + 1.001f, coord.z + 1.001f));

		_voxLines.AddLine(Vector3(coord.x - 0.001f, coord.y + 1.001f, coord.z + 1.001f),
						  Vector3(coord.x - 0.001f, coord.y + 1.001f, coord.z - 0.001f));
		_voxLines.AddLine(Vector3(coord.x - 0.001f, coord.y + 1.001f, coord.z + 1.001f),
						  Vector3(coord.x - 0.001f, coord.y - 0.001f, coord.z + 1.001f));
		_voxLines.AddLine(Vector3(coord.x - 0.001f, coord.y + 1.001f, coord.z + 1.001f),
						  Vector3(coord.x + 1.001f, coord.y + 1.001f, coord.z + 1.001f));

		_voxLines.AddLine(Vector3(coord.x + 1.001f, coord.y + 1.001f, coord.z - 0.001f),
						  Vector3(coord.x - 0.001f, coord.y + 1.001f, coord.z - 0.001f));
		_voxLines.AddLine(Vector3(coord.x + 1.001f, coord.y + 1.001f, coord.z - 0.001f),
						  Vector3(coord.x + 1.001f, coord.y - 0.001f, coord.z - 0.001f));
		_voxLines.AddLine(Vector3(coord.x + 1.001f, coord.y + 1.001f, coord.z - 0.001f),
						  Vector3(coord.x + 1.001f, coord.y + 1.001f, coord.z + 1.001f));
		glLineWidth(10.0f);
		renderer->SetModelMatrix(Matrix4::identity);
		_voxLines.Draw(renderer);
		glLineWidth(2.0f);
	}
}

void
PlayerController::OnInit()
{
	PhysicalObject::OnInit();
	_voxLines.Init();
}

void 
PlayerController::OnUpdate()
{
	PhysicalObject::OnUpdate();
	assert(_octree != nullptr, "Octree pointer is null!");
	ChangeBlocks();
	Move();

	_camera->SetPosition(_transform.GetWorldPosition());
	_camera->Move(Vector3(0.0f, 0.6f, 0.0f));
}

void
PlayerController::ChangeBlocks()
{
	rayInfo = RayIntersection();
	Ray cameraRay(_camera->GetPosition(), _camera->GetDirection(), digDistance);
	_octree->CheckRayCollision(&cameraRay, &rayInfo);

	static float prevTime = 0;
	if (Input::IsPressed("Attack1")) {
		if (Time::GetTime() - prevTime > 0.2f) {
			prevTime = Time::GetTime();
			if (rayInfo.CollisionFound()) {
				Chunk* hitCh = rayInfo.GetCollidedChunk();
				const Vector3& coord = rayInfo.GetVoxelCoordinates();
				hitCh->Set(coord, Voxel::NONE);
			}
		}
	}

	static Vector3 planes[6] = {
		Vector3::forward,
		Vector3::backward,
		Vector3::right,
		Vector3::left,
		Vector3::up,
		Vector3::down
	};

	if (Input::IsPressed("Attack2")) {
		if (Time::GetTime() - prevTime > 0.2f) {
			prevTime = Time::GetTime();
			if (rayInfo.CollisionFound()) {
				Chunk* hitCh = rayInfo.GetCollidedChunk();
				const Vector3& coord = rayInfo.GetVoxelCoordinates();

				float dots[6];
				for (int i = 0; i < 6; ++i) {
					dots[i] = Vector3::Dot(cameraRay.GetDirection(), planes[i]);
				}
				float minimum = 1;
				int ind = 0;
				for (int i = 0; i < 6; ++i) {
					if (dots[i] < 0.0f && dots[i] < minimum) {
						minimum = dots[i];
						ind = i;
					}
				}

				Vector3 constraintLow = hitCh->GetOffset();
				Vector3 constraintHigh = constraintLow + (float)(Chunk::dimension - 1);
				Vector3 newCoords = coord + planes[ind];
				_octree->Insert(Voxel(Voxel::WOOD), newCoords);
			}
		}
	}

}

void
PlayerController::Move()
{
	Vector2 rot = Input::GetMouseOffset();
	_camera->Rotate(rot.x, -rot.y);

	Vector3 front = Vector3::right;

	Quaternion::RotatePoint(Quaternion().SetRotationY(_camera->GetHorizontal()).Conjugate(), &front);

	Vector3 right = front;
	Quaternion::RotatePoint(Quaternion().SetRotationY(-90.0f), &right);

	float run = 1.0f;
	if (Input::IsHolded("Walk"))
		run = runModifier;

	if (Input::IsHolded("Forward"))
		_transform.GetPosition() += front * speed * run * Time::DeltaTime();

	if (Input::IsHolded("Back"))
		_transform.GetPosition() += -front * speed * run * Time::DeltaTime();

	if (Input::IsHolded("Right"))
		_transform.GetPosition() += right * speed * run * Time::DeltaTime();

	if (Input::IsHolded("Left"))
		_transform.GetPosition() += -right * speed * run * Time::DeltaTime();

	if (_grounded) {
		if (Input::IsHolded("Jump")) {
			AddForce(Vector3::up * jumpForce/_mass);
		}
	}
}

}