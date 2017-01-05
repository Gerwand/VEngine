#include "MeshedObject.h"


namespace vengine {

MeshedObject::MeshedObject(const std::string& name) : GameObject(name), _mesh(0)
{
}

MeshedObject::MeshedObject(const MeshedObject& source) : GameObject(source), _mesh(source._mesh)
{
}

void
MeshedObject::OnDraw(Renderer *renderer)
{
	if (_mesh != 0) {
		renderer->SetModelMatrix(_transform.GetModelMatrix());
		meshManager.Draw(_mesh, renderer);
	}
}

}