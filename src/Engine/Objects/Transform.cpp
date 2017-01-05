#include "Transform.h"

namespace vengine {

Transform::Transform() :
	_position(Vector3::zeroes),
	_scale(Vector3::ones),
	_rotation(Quaternion::identity),
	_lastPosition(Vector3::zeroes),
	_lastScale(Vector3::ones),
	_lastRotation(Quaternion::identity),
	_modelMatrix(Matrix4::identity),
	_localModelMatrix(Matrix4::identity),
	_parent(NULL),
	_recalculated(false)
{
}

const Matrix4&
Transform::GetModelMatrix()
{
	UpdateMatrix();
	
	return _modelMatrix;
}

void 
Transform::UpdateMatrix()
{
	bool changed = false;
	if (_lastPosition != _position || _lastScale != _scale || _lastRotation != _rotation)
		changed = true;

	if (changed) {
		_localModelMatrix = Matrix4::GetTranslate(_position) * _rotation * Matrix4::GetScale(_scale);
		_recalculated = true;
	}

	if (_parent) {
		if (_parent->_recalculated || changed) {
			_modelMatrix = _parent->_modelMatrix * _localModelMatrix;
			_recalculated = true;
		}
	}
	else if (changed) {
		_modelMatrix = _localModelMatrix;
		_recalculated = true;
	}

	_lastPosition = _position;
	_lastScale = _scale;
	_lastRotation = _rotation;

}

void 
Transform::Reset()
{
	_position = Vector3::zeroes;
	_scale = Vector3::ones;
	_rotation = Quaternion::identity;
}

}