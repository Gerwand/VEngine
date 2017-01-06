#include "MeshedObject.h"


namespace vengine {


void
MeshedObject::OnDraw(Renderer *renderer)
{
	if (_mesh != 0) {
		renderer->SetModelMatrix(_transform.GetModelMatrix());
		meshManager.Draw(_mesh, renderer);
	}
}

}