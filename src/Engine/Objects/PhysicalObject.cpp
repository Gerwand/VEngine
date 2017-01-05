#include "PhysicalObject.h"

namespace vengine {
Vector3 PhysicalObject::_gravityForce = Vector3::down * 9.81f;
extern bool debugDraw;

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
	_collider.SetPosition(_transform.GetWorldPosition());
	_transform.UpdateMatrix();
}

void 
PhysicalObject::OnUpdate()
{

}

void 
PhysicalObject::OnCollision(const CollisionInfo& collision)
{
	if (IsStatic())
		return;

	if (collision.GetOtherObject() == this)
		return;

	//printf("\nCollision %s!", GetName().c_str());

	if (collision.HasCollidedWithTerrain()) {
		const Directions& dirs = collision.GetHittedDirections();

		const Vector3 dir = _transform.GetLastPosition() - _transform.GetPosition();
		const Vector3& last = _transform.GetLastPosition();
		if (collision.HasCollidedWithTerrain(CollisionInfo::BOTTOM | CollisionInfo::TOP)) {
			_velocity.y =  -_velocity.y * _bounciness;
			_transform.GetPosition().y = last.y;
		}

		if (collision.HasCollidedWithTerrain(CollisionInfo::NORTH ) && dir.z > 0.0) {
			_velocity.z =  -_velocity.z * _bounciness;
			_transform.GetPosition().z = last.z;
		}
		else if (collision.HasCollidedWithTerrain(CollisionInfo::SOUTH ) && dir.z < 0.0) {
			_velocity.z =  -_velocity.z * _bounciness;
			_transform.GetPosition().z = last.z;
		}

		if (collision.HasCollidedWithTerrain(CollisionInfo::EAST) && dir.x > 0.0) {
			_velocity.x = -_velocity.z * _bounciness;
			_transform.GetPosition().x = last.x;
		}
		else if (collision.HasCollidedWithTerrain(CollisionInfo::WEST ) && dir.x < 0.0) {
			_velocity.x = -_velocity.z * _bounciness;
			_transform.GetPosition().x = last.x;
		}

		_grounded = false;
		if (collision.HasCollidedWithTerrain(CollisionInfo::BOTTOM))
			_grounded = true;
	}
	//std::cout << "\n";
	
	if (collision.HasCollidedWithObject()) {
		PhysicalObject* other = (PhysicalObject *)collision.GetOtherObject();
		assert(other != nullptr, "Other object is null");
		
		if (!other->IsStatic()) {
			Vector3 tmp = _velocity;
			_velocity += other->_mass * other->_velocity * Time::DeltaTime();
			other->_velocity += _mass * tmp * Time::DeltaTime();
		}
		else {
			_velocity = Vector3::zeroes;
		}
	}
	
	
	//_transform.SetPosition(_transform.GetLastPosition());
	_collider.SetPosition(_transform.GetWorldPosition());
}

void
PhysicalObject::OnLateUpdate()
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
	if (debugDraw) {
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