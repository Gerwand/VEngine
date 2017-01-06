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
#include "Resources/Renderables/VoxelMesh.h"
#include "Resources/Voxels/VoxelArray3D.h"
#include "Objects/GameObject.h"
#include "Objects/MeshedObject.h"
#include "Objects/PhysicalObject.h"
#include "Objects/PlayerController.h"
#include "Objects/World.h"
#include "Octree.h"
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