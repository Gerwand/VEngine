#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

#include "Singleton.h"

#include "Window.h"
#include "Input.h"
#include "KeyBindings.h"

#include "Managers/ShaderManager.h"
#include "Managers/GlProgramManager.h"
#include "Managers/GlPipelineManager.h"
#include "Managers/ShaderFileManager.h"
#include "Managers/TextureManager.h"
#include "Managers/MeshManager.h"
#include "Managers/VoxelArrayManager.h"

#include "Managers/GlBuffer.h"
#include "Managers/VertexArray.h"
#include "Vertex.h"
#include "Time.h"
#include "VEMath.h"
#include "CameraFPP.h"
#include "Mesh.h"
#include "VoxelMesh.h"
#include "VoxelArray3D.h"
#include "Engine/GameObject.h"
#include "Engine/MeshedObject.h"
#include "Engine/PhysicalObject.h"
#include "Engine/PlayerController.h"
#include "Engine/World.h"
#include "Octree.h"
#include "TerrainGenerator.h"

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
	void LoadWorld();

	void Draw();

	static void ErrorHandler(int error, const char* description);
};

#define vEngine Singleton<VEngine>::GetSingleton()
//#define Time Engine.GetTime();
}