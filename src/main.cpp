#include "Engine/VEngine.h"

using namespace vengine;

VEngine engine;

int main()
{
	if (engine.Init("VEngine")) {
		std::cout << "\nInit failed!\n";
		return VE_FAULT;
	}

	if (engine.Load()) {
		return VE_FAULT;
	}

	engine.Run();

	return 0;
}