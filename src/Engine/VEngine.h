#pragma once

#include "Singleton.h"

#include "IO/Window.h"
#include "IO/Input.h"
#include "KeyBindings.h"
#include "VoxelModels.h"

#include "Resources/Managers/ShaderManager.h"
#include "Resources/Managers/GlProgramManager.h"
#include "Resources/Managers/GlPipelineManager.h"
#include "Resources/Managers/ShaderFileManager.h"
#include "Resources/Managers/TextureManager.h"
#include "Resources/Managers/MeshManager.h"
#include "Resources/Managers/VoxelArrayManager.h"
#include "Resources/Managers/GameObjectManager.h"

#include "Vertex.h"
#include "Time.h"
#include "VEMath.h"
#include "CameraFPP.h"
#include "Canvas.h"
#include "Resources/UI/ToggleButton.h"
#include "Resources/Renderables/VoxelMesh.h"
#include "Resources/Renderables/GUIElement.h"
#include "Resources/Voxels/VoxelArray3D.h"
#include "Objects/GameObject.h"
#include "Objects/MeshedObject.h"
#include "Objects/PhysicalObject.h"
#include "Objects/PlayerController.h"
#include "Objects/Projectile.h"
#include "Objects/PlayerHead.h"
#include "Objects/Enemy.h"
#include "Objects/Enemyhead.h"
#include "Objects/World.h"
#include "Octree.h"
#include "DebugConfig.h"
#include "TerrainGenerator.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

namespace vengine {

class VEngine : public Singleton<VEngine>
{
public:
	VEngine();
	~VEngine();

	int Init(const std::string& gameTitle);
	Renderer& GetRenderer();

	int Load();

	void Destroy();

	void Run();

private:
	Renderer _renderer;
	std::string _gameTitle;
	GameObject* _world;
	Octree _octree;
	Canvas* _menuGui;

#ifdef VE_DEBUG
	DebugConfig _debugConfig;
#endif
	ToggleButton* _octButton;
	ToggleButton* _colButton;
	ToggleButton* _posButton;
	ToggleButton* _wirButton;

	void DestroyWorld();
	void DestroyOtherManagers();
	void DestroyFileManagers();
	void DestroyResourceManagers();
	void DestroyGLFWLibrary();

	int InitOtherManagers();
	int InitFileManagers();
	int InitResourceManagers();
	int InitGLFWLibrary();
	int InitWindow();
	int InitGLFWCallbacks();
	int InitLocalResources();

	int LoadResources();
	int LoadShaders();
	int LoadTextures();
	int LoadInput();
	void LoadObjects();
	void LoadWorld();
	
	void Draw();

	static void ErrorHandler(int error, const char* description);
};

#define vEngine Singleton<VEngine>::GetSingleton()
}