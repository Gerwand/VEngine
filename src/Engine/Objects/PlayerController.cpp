#include "PlayerController.h"

namespace vengine {
/* Tool names for game objects that are stored in gameObjectManager */
std::string PlayerController::_toolNames[7]{
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

	/* If ray hit someting, we must draw box around this voxel */
	if (_rayInfo.CollisionFound()) {
		Chunk* hitCh = _rayInfo.GetCollidedChunk();
		const Vector3& coord = _rayInfo.GetVoxelCoordinates();
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

	/* If there is something in the arm, destroy it. */
	if (_arm->HasChild())
		GameObject::Destroy((GameObject *)_arm->GetChild());

	if (_tool == NONE)
		return;

	/* Get proper tool and attach it to the arm */
	const std::string& name = _toolNames[_tool - 1];
	GameObject* tool = gameObjectManager.Instantiate(name);
	_arm->Attach(tool);
}


void
PlayerController::ChangeBlocks()
{
	_rayInfo = RayIntersection();
	/* Create ray depending on looking direction */
	Ray cameraRay(_camera->GetPosition(), _camera->GetDirection(), _digDistance);
	_octree->CheckRayCollision(&cameraRay, &_rayInfo);


	/* Static planes for checking where to build block */
	static const Vector3 planes[6] = {
		Vector3::forward,
		Vector3::backward,
		Vector3::right,
		Vector3::left,
		Vector3::up,
		Vector3::down
	};

	/* Used to reduce speed of digging/placing to 5 times every each second */
	static float prevTime = 0;
	if (Input::IsHolded("Attack1")) {

		if (Time::GetTime() - prevTime > 0.2f) {
			/* Update time */
			prevTime = Time::GetTime();

			/* Initialize variables */
			Chunk* hitCh = _rayInfo.GetCollidedChunk();
			const Vector3& coord = _rayInfo.GetVoxelCoordinates();
			Quaternion quat1, quat2;
			PhysicalObject* projectile;
			Transform transform;
			Vector3 dir;
			switch (_tool) {
			case SWORD:
				/* If there is sword in the arm, we want to throw it */
				projectile = (PhysicalObject *)gameObjectManager.Instantiate("Projectile");

				/* Sword should point in the same direction that the camera is */
				quat1.SetRotation(-_camera->GetVertical() + 90.0f, Vector3::forward);
				quat2.SetRotation(-_camera->GetHorizontal(), Vector3::up);

				transform.SetRotation(quat2*quat1);
				transform.SetPosition(_camera->GetPosition());
				/* Setup transformations of the projectile */
				projectile->SetTransform(transform);
				/* And attach it to the world */
				projectile->AttachTo(GetRoot());

				/* Add force! */
				projectile->AddForce(_camera->GetDirection() * 20.0f);
				/* And add to the octree for collision checks*/
				_octree->Add(projectile);
				break;
			case PICKAXE:
				/* If we hit something, we want to delete things */
				if (_rayInfo.CollisionFound()) {
					hitCh->Set(coord, Voxel::NONE);
				}
				break;
			case DIRT:
			case WOOD:
			case STONE:
			case GRASS:
				if (_rayInfo.CollisionFound()) {
					/* If we are building something, we want first to determine where to place next voxel */
					float dots[6];

					/* We are creating dot products with player distance from the voxel and all planes */
					dir = (coord + Vector3(0.5f)) - cameraRay.GetStart();
					for (int i = 0; i < 6; ++i) {
						dots[i] = Vector3::Dot(dir, planes[i]);
					}

					float minimum = 1;
					/* We are looking for minimal dot product. It will be the closes opposite direction to player->voxel direction */
					int ind = 0;
					for (int i = 0; i < 6; ++i) {
						if (dots[i] < 0.0f && dots[i] < minimum) {
							minimum = dots[i];
							ind = i;
						}
					}

					Vector3 constraintLow = hitCh->GetOffset();
					Vector3 constraintHigh = constraintLow + (float)(Chunk::dimension - 1);
					/* We want to place block in opposite direction to player->voxel direction */
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
	/* 0 Angle if at +x, so it will be front */
	Vector3 front = Vector3::right;
	/* We want to rotate front direction accordint to horizontal rotation. We must conjugate it because xz plane is different than xy */
	Quaternion::RotatePoint(Quaternion().SetRotationY(_camera->GetHorizontal()).Conjugate(), &front);

	/* Same for right vector for strafing */
	Vector3 right = front;
	Quaternion::RotatePoint(Quaternion().SetRotationY(-90.0f), &right);

	float run = 1.0f;
	if (Input::IsHolded("Walk"))
		run = _runModifier;

	if (Input::IsHolded("Forward"))
		_transform.GetPosition() += front * _speed * run * Time::DeltaTime();

	if (Input::IsHolded("Back"))
		_transform.GetPosition() += -front * _speed * run * Time::DeltaTime();

	if (Input::IsHolded("Right"))
		_transform.GetPosition() += right * _speed * run * Time::DeltaTime();

	if (Input::IsHolded("Left"))
		_transform.GetPosition() += -right * _speed * run * Time::DeltaTime();

	/* If player is grounded, we can jump */
	if (_grounded) {
		if (Input::IsHolded("Jump")) {
			AddForce(Vector3::up * _jumpForce);
		}
	}
}

}