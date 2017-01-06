#include "Engine/VEngine.h"

using namespace vengine;

VEngine engine;

int main()
{
	engine.Init("VEngine");

	if (engine.Load()) {
		return VE_FAULT;
	}

	engine.Run();

	return 0;
}