#include "PlayerController.h"

namespace vengine {
std::string PlayerController::toolNames[7]{
	"Sword",
	"Pickaxe",
	"Dirt",
	"Grass",
	"Wood",
	"Stone"
};

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
	
	if (Input::GetCursorMode())
		return;

	ChooseTool();
	ChangeBlocks();
	Move();
}

void
PlayerController::ChooseTool()
{
	Tools previousTool = _tool;
	if (Input::IsPressed(GLFW_KEY_1)) {
		_tool = SWORD;
	}
	else if (Input::IsPressed(GLFW_KEY_2)) {
		_tool = PICKAXE;
	}
	else if (Input::IsPressed(GLFW_KEY_3)) {
		_tool = DIRT;
	}
	else if (Input::IsPressed(GLFW_KEY_4)) {
		_tool = GRASS;
	}
	else if (Input::IsPressed(GLFW_KEY_5)) {
		_tool = WOOD;
	}
	else if (Input::IsPressed(GLFW_KEY_6)) {
		_tool = STONE;
	}

	if (previousTool != _tool)
		AttachTool();
}

void 
PlayerController::AttachTool() {
	assert(_arm != nullptr, "Arm is null!");

	if (_arm->HasChild())
		GameObject::Destroy((GameObject *)_arm->GetChild());

	if (_tool == NONE)
		return;

	const std::string& name = toolNames[_tool - 1];
	GameObject* tool = gameObjectManager.Instantiate(name);
	_arm->Attach(tool);
}


void
PlayerController::ChangeBlocks()
{
	rayInfo = RayIntersection();
	Ray cameraRay(_camera->GetPosition(), _camera->GetDirection(), digDistance);
	_octree->CheckRayCollision(&cameraRay, &rayInfo);

	static float prevTime = 0;
	static Vector3 planes[6] = {
		Vector3::forward,
		Vector3::backward,
		Vector3::right,
		Vector3::left,
		Vector3::up,
		Vector3::down
	};

	if (Input::IsHolded("Attack1")) {
		if (Time::GetTime() - prevTime > 0.2f) {
			prevTime = Time::GetTime();
			Chunk* hitCh = rayInfo.GetCollidedChunk();
			const Vector3& coord = rayInfo.GetVoxelCoordinates();
			Quaternion quat1, quat2;
			PhysicalObject* projectile;
			Transform transform;
			switch (_tool) {
			case SWORD:
				projectile = (PhysicalObject *)gameObjectManager.Instantiate("Projectile");
				quat1.SetRotation(-_camera->GetVertical() + 90.0f, Vector3::forward);
				quat2.SetRotation(-_camera->GetHorizontal(), Vector3::up);
				transform.SetRotation(quat2*quat1);
				transform.SetPosition(_camera->GetPosition());
				projectile->SetTransform(transform);
				projectile->AttachTo(GetRoot());
				projectile->AddForce(_camera->GetDirection() * 20.0f);
				_octree->Add(projectile);
				break;
			case PICKAXE:
				if (rayInfo.CollisionFound()) {
					hitCh->Set(coord, Voxel::NONE);
				}
				break;
			case DIRT:
			case WOOD:
			case STONE:
			case GRASS:
				if (rayInfo.CollisionFound()) {
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
					_octree->Insert(Voxel(_tool - 2), newCoords);
					break;
				}
			}
		}
	}
}

void
PlayerController::Move()
{
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
			AddForce(Vector3::up * jumpForce);
		}
	}
}

}