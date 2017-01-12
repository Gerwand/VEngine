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

/*
* VEngine - Voxel Engine is the class that manages all components and synchronizes them. 
* It is loading all resources and all objects. 
*/
class VEngine : public Singleton<VEngine>
{
public:
	/* Destructor is destroying all allocated resources */
	~VEngine();

	/* Initializes all resources and sets game title */
	int Init(const std::string& gameTitle);

	/* Load all resources needed for the game */
	int Load();

	/* Destroy all resources */
	void Destroy();

	/* Run main loop. It will initalize objects at first. */
	void Run();

private:
	Renderer _renderer;		/* Used for rendering objects */
	std::string _gameTitle;	/* Title of the game */
	GameObject* _world;		/* This object represents scene - all game objects will be attached to this */
	Octree _octree;			/* Octree used for collision checking and sorting physical objects and chunks */
	Canvas* _menuGui;		/* Canvas storing GUI for stering debugging options */

#ifdef VE_DEBUG
	DebugConfig _debugConfig; /* Configuration for debug drawing */
#endif

	ToggleButton* _octButton;	/* Button for drawing octree boundaries */
	ToggleButton* _colButton;	/* Button for drawing colliders boundaries */
	ToggleButton* _posButton;	/* Button for drawing positions of the objects */
	ToggleButton* _wirButton;	/* Button for drawing objects as wireframe */

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
	
	/* Routine that is preparing everything for drawing */
	void Draw();

	static void ErrorHandler(int error, const char* description);
	Renderer& GetRenderer();
};

#define vEngine Singleton<VEngine>::GetSingleton()
}