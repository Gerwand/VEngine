#include "PhysicalObject.h"

namespace vengine {
Vector3 PhysicalObject::_gravityForce = Vector3::down * 9.81f;

PhysicalObject::PhysicalObject(const std::string& name) :
	MeshedObject(name),
	_velocity(Vector3::zeroes), _acceleration(Vector3::zeroes),
	_colliderOffset(Vector3::zeroes)
{
	_isStatic = false;
}


PhysicalObject::PhysicalObject(const PhysicalObject& source) : MeshedObject(source)
{
	_collider = source._collider;
	_isStatic = source._isStatic;

	_velocity = source._velocity;
	_acceleration = source._acceleration;
	_colliderOffset = source._colliderOffset;
}

void 
PhysicalObject::OnInit()
{
	_collider.SetPosition(_transform.GetWorldPosition() + _colliderOffset);
	_transform.UpdateMatrix();
}

void 
PhysicalObject::OnCollision(const CollisionInfo& collision)
{
	if (IsStatic())
		return;

	if (collision.GetOtherObject() == this)
		return;

	Vector3 newPos = _transform.GetPosition();
	const Vector3 dir = _transform.GetPosition() - _transform.GetLastPosition();
	const Vector3& last = _transform.GetLastPosition();

	if (collision.HasCollidedWithTerrain()) {
		const Directions& dirs = collision.GetHittedDirections();

		if (collision.HasCollidedWithTerrain(CollisionInfo::BOTTOM | CollisionInfo::TOP)) {
			_velocity.y =  -_velocity.y * _bounciness;
			newPos.y = last.y;
		}

		if (collision.HasCollidedWithTerrain(CollisionInfo::NORTH ) && dir.z < 0.0) {
			_velocity.z =  -_velocity.z * _bounciness;
			newPos.z = last.z;
		}
		else if (collision.HasCollidedWithTerrain(CollisionInfo::SOUTH ) && dir.z > 0.0) {
			_velocity.z =  -_velocity.z * _bounciness;
			newPos.z = last.z;
		}

		if (collision.HasCollidedWithTerrain(CollisionInfo::EAST) && dir.x < 0.0) {
			_velocity.x = -_velocity.z * _bounciness;
			newPos.x = last.x;
		}
		else if (collision.HasCollidedWithTerrain(CollisionInfo::WEST ) && dir.x > 0.0) {
			_velocity.x = -_velocity.z * _bounciness;
			newPos.x = last.x;
		}

		_grounded = false;
		if (collision.HasCollidedWithTerrain(CollisionInfo::BOTTOM))
			_grounded = true;
	}

	if (collision.HasCollidedWithObject()) {
		PhysicalObject* other = (PhysicalObject *)collision.GetOtherObject();
		assert(other != nullptr, "Other object is null");

		const Vector3 relPos = other->GetCollider().GetPosition() - _collider.GetPosition();

		if (dir.x * relPos.x >= 0.0f) {
			newPos.x = last.x;
			_velocity.x = 0.0f;
		}
		if (dir.y * relPos.y >= 0.0f) {
			newPos.y = last.y;
			_velocity.y = 0.0f;
		}
		if (dir.z * relPos.z >= 0.0f) {
			newPos.z = last.z;
			_velocity.z = 0.0f;
		}
	}
	
	
	_transform.SetPosition(newPos);
	_collider.SetPosition(_transform.GetWorldPosition());
}

void
PhysicalObject::OnPhysic()
{
	if (!IsStatic()) {
		UpdatePhysic();
		if (HasChanged()) {
			_collider.SetPosition(_transform.GetWorldPosition());
		}
	}
}

void 
PhysicalObject::AddForce(const Vector3& force)
{
	_acceleration += force / _mass;
}

void
PhysicalObject::UpdatePhysic()
{
	if(!_grounded)
		_acceleration += _gravityForce * _mass * Time::DeltaTime();
	_velocity += _acceleration;

	if (_grounded) {
		Vector3 friction = _terrainFriction * Vector3(-_velocity.x * _mass * _gravityForce.y, 0.0f, -_velocity.z * _mass * _gravityForce.y);
		_velocity += friction;
	}

	_acceleration = Vector3::zeroes;

	_transform.GetPosition() += _velocity * Time::DeltaTime();
	_grounded = false;
}

void 
PhysicalObject::OnDraw(Renderer* renderer) 
{
	MeshedObject::OnDraw(renderer);

#ifdef VE_DEBUG
	if (debugConfig->drawColliders) {
		Lines boxes;
		std::vector<Vector3> lines;
		boxes.Init();
		_collider.InsertLines(&lines);

		boxes.SetColor(Vector3(0.0f, 0.0f, 1.0f));

		for (std::vector<Vector3>::iterator it = lines.begin(); it < lines.end();) {
			const Vector3& start = *it;
			++it;
			const Vector3& end = *it;
			++it;
			boxes.AddLine(start, end);
		}
		renderer->SetModelMatrix(Matrix4::identity);
		boxes.Draw(renderer);
	}
#endif

}

}