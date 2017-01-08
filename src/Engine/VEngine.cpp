#include "VEngine.h"

namespace vengine {

VEngine::VEngine() {}
VEngine::~VEngine()
{
	Destroy();
}

int
VEngine::Init(const std::string& gameTitle)
{
	_gameTitle = gameTitle;
	int error;

	error = InitGLFWLibrary();
	if (error) {
		printf("Cannot initialize GLFW library. Error code: %d", error);
		goto glfw_err;
	}

	error = InitFileManagers();
	if (error) {
		printf("Cannot initialize file managers. Error code: %d", error);
		goto fileman_err;
	}

	error = InitWindow();
	if (error) {
		printf("Cannot initialize file managers. Error code: %d", error);
		goto callbacks_err;
	}
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION);

	error = InitGLFWCallbacks();
	if (error) {
		printf("Cannot assign GLFW callbacks. Error code: %d", error);
		goto callbacks_err;
	}

	error = InitOtherManagers();
	if (error) {
		printf("Cannot initialize other managers. Error code: %d", error);
		goto callbacks_err;
	}

	error = InitResourceManagers();
	if (error) {
		printf("Cannot initialize resource managers. Error code: %d", error);
		goto resman_err;
	}

	error = InitLocalResources();
	if (error) {
		printf("Cannot initialize local resources. Error code: %d", error);
		goto locres_err;
		;
	}

	return 0;


locres_err:
	DestroyResourceManagers();
resman_err:
	DestroyOtherManagers();
callbacks_err:
	DestroyFileManagers();
fileman_err:
	DestroyGLFWLibrary();
glfw_err:
	return error;
}

int
VEngine::InitLocalResources()
{
	_renderer.Init();
	_menuGui = new Canvas(Vector4(0.0f, 0.0f, 0.0f, 0.7f));
	GameObject::debugConfig = &_debugConfig;
	_debugConfig.drawColliders = false;
	_debugConfig.drawOctree = false;
	_debugConfig.drawPositions = false;

	return 0;
}

int
VEngine::InitFileManagers()
{
	try {
		new ShaderFileManager;
	}
	catch (std::bad_alloc err) {
		DestroyFileManagers();
		return VE_EMEM;
	}

	return 0;
}
int
VEngine::InitOtherManagers()
{
	return 0;
}
int
VEngine::InitResourceManagers()
{
	try {
		new ShaderManager;
		new GlProgramManager;
		new GlPipelineManager;
		new TextureManager;
		new MeshManager;
		new VoxelArrayManager;
		new GameObjectManager;
	}
	catch (std::bad_alloc err) {
		DestroyResourceManagers();
		return VE_EMEM;
	}

	return 0;
}
int
VEngine::InitGLFWLibrary()
{
	if (!glfwInit())
		return VE_FAULT;
	return 0;
}
int
VEngine::InitWindow()
{
	//Read resolution from config file
	Window::Init(1366, 768, _gameTitle.c_str());
	if (!Window::CreateWindowed())
		return VE_FAULT;
	Window::MakeActiveContext();
	return 0;
}
int
VEngine::InitGLFWCallbacks()
{
	glfwSetErrorCallback(ErrorHandler);
	return 0;
}

int
VEngine::Load()
{
	int error;
	error = LoadInput();
	if (error) {
		fprintf(stderr, "Failed to load Input! Error: %d\n", error);
		return error;
	}
	error = LoadResources();
	if (error) {
		fprintf(stderr, "Failed to load resources! Error: %d\n", error);
		return error;
	}
	return 0;
}

int
VEngine::LoadInput()
{
	Input::Init(keyNames, keyCodes, bindNum);
	Input::SetMode(Input::KEY_MODE);
	Input::DisableCursor();

	return 0;
}


int
VEngine::LoadResources()
{
	int error;
	error = LoadShaders();
	if (error)
		return error;
	error = LoadTextures();
	if (error)
		return error;
	LoadObjects();
	LoadWorld();

	return 0;
}

int
VEngine::LoadShaders()
{
	
	int error;

	error = _renderer.AddShader("VertexSimple", "Shaders/Simple.vert", Shader::VERTEX, Renderer::STANDARD);
	if (error) {
		std::cout << "Adding failed for VertexSimple - Shaders/Simple.vert" << std::endl;
		return error;
	}

	error = _renderer.AddShader("FragSimple", "Shaders/Simple.frag", Shader::FRAGMENT, Renderer::STANDARD);
	if (error) {
		std::cout << "Adding failed for FragSimple - Shaders/Simple.frag" << std::endl;
		return error;
	}

	error = _renderer.AddShader("VertexVoxel", "Shaders/Voxel.vert", Shader::VERTEX, Renderer::VOXEL);
	if (error) {
		std::cout << "Adding failed for VertexVoxel - Shaders/Simple.vert" << std::endl;
		return error;
	}

	error = _renderer.AddShader("FragVoxel", "Shaders/Voxel.frag", Shader::FRAGMENT, Renderer::VOXEL);
	if (error) {
		std::cout << "Adding failed for FragVoxel - Shaders/Simple.frag" << std::endl;
		return error;
	}

	error = _renderer.AddShader("VertexGUI", "Shaders/GUI.vert", Shader::VERTEX, Renderer::GUI);
	if (error) {
		std::cout << "Adding failed for VertexGUI - Shaders/GUI.vert" << std::endl;
		return error;
	}

	error = _renderer.AddShader("FragGUI", "Shaders/GUI.frag", Shader::FRAGMENT, Renderer::GUI);
	if (error) {
		std::cout << "Adding failed for FragGUI - Shaders/GUI.frag" << std::endl;
		return error;
	}
	return 0;
}

int 
VEngine::LoadTextures()
{
	unsigned int tex = textureManager.GetTexture("WoodOld");
	int rc = textureManager.LoadTexture(tex, "Textures/wood.jpg");
	if (rc) {
		std::cout << "Failed to load texture wood.jpg" << std::endl;
		return rc;
	}
	
	tex = textureManager.GetTexture("Atlas");
	rc = textureManager.LoadTexture(tex, "Textures/tex.tga");
	if (rc) {
		std::cout << "Failed to load texture tex.tga" << std::endl;
		return rc;
	}

	VoxelMesh::SetAtlas(tex);
	_renderer.SetAtlasTileSize(1.0f / Voxel::texsPerRow);
	return 0;
}


void
VEngine::LoadObjects()
{
	/* Create meshes and VA of static objects */
	VoxelMesh* mesh = new VoxelMesh;
	mesh->Init("Sword");

	int vaSword = voxelArrayManager.GetVoxelArray("VoxelSword");
	voxelArrayManager.SetDimension(vaSword, swordDim.x, swordDim.y, swordDim.z);
	voxelArrayManager.SetVoxels(vaSword, swordVoxels);
	voxelArrayManager.SetVoxelSize(vaSword, swordSize);
	voxelArrayManager.GenerateMesh(vaSword, mesh);
	unsigned int swordMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("Cube");
	int cubeVox = voxelArrayManager.GetVoxelArray("Cube");
	voxelArrayManager.SetDimension(cubeVox, cubeDim.x, cubeDim.y, cubeDim.z);
	voxelArrayManager.SetVoxels(cubeVox, cube);
	voxelArrayManager.SetVoxelSize(cubeVox, cubeSize);
	voxelArrayManager.GenerateMesh(cubeVox, mesh);
	unsigned int cubeMesh = meshManager.AddMesh(mesh);

	/* Objects initialization */
	Transform transformMain;
	transformMain.SetPosition(Vector3(0.0f, 30.0f, 0.0f));
	/* Sword object initialization */
	PhysicalObject* sword = new PhysicalObject("Sword");
	sword->SetMesh(swordMesh);
	sword->SetTransform(transformMain);
	sword->GetCollider().SetPosition(transformMain.GetPosition());
	int x, y, z;
	voxelArrayManager.GetDimension(vaSword, &x, &y, &z);
	sword->GetCollider().SetDimension(Vector3((float)x, (float)y, (float)z) * voxelArrayManager.GetVoxelSize(vaSword));

	gameObjectManager.AddGameObject(sword);
}

void 
VEngine::LoadWorld()
{
	_renderer.SetClearColor(Vector3(135 / 255.0f, 191 / 255.0f, 255/255.0f));

	_world = new World("World");
	_world->Rename("World");

	/* Initialize player */
	Transform transformPlayer;
	transformPlayer.SetPosition(Vector3(4.0f, 50.0f, 4.0f));

	PlayerController* player = new PlayerController;
	CameraFPP* camera = new CameraFPP;	

	player->SetCamera(camera);
	player->SetTag("Player");
	_renderer.SetActiveCamera(camera);
	
	player->GetCollider().SetDimension(Vector3(0.5f, 1.5f, 0.5f));
	player->AttachTo(_world);
	player->SetTransform(transformPlayer);
	player->SetOctree(&_octree);
	_octree.Add(player);
	
	/* Spawn objects */
	Transform transform;
	transform.SetPosition(Vector3(0.0f, 30.0f, 0.0f));
	PhysicalObject* sword = (PhysicalObject *)gameObjectManager.Instantiate("Sword");
	sword->SetTransform(transform);
	sword->AttachTo(_world);
	_octree.Add(sword);

	transform.SetPosition(Vector3(0.0f, 50.0f, 0.0f));
	sword = (PhysicalObject *)gameObjectManager.Instantiate("Sword");
	sword->SetTransform(transform);
	sword->AttachTo(_world);
	_octree.Add(sword);

	/* Generate terrain */
	TerrainGenerator terrainGen(0, 5, 1);
	terrainGen.SetSmoothness(256);
	terrainGen.SetDetails(1);
	terrainGen.SetSpread(32);
	terrainGen.SetSeed(58230947u);

	for (int z = 0; z < 16; ++z) {
		for (int y = 0; y < 16; ++y) {
			for (int x = 0; x < 16; ++x) {
				Chunk* chunk = new Chunk(Vector3((x - 8.0f) * Chunk::dimension, (y - 8.0f) * Chunk::dimension, (z - 8.0f) * Chunk::dimension));
				if(terrainGen.GetChunk(chunk))
					_octree.Add(chunk);
			}
		}
	}
	_octButton = new ToggleButton(Vector2(50.0f, 50.0f), Vector2(200.0f, 100.0f), Vector4(0.1f, 0.2f, 1.0f, 0.8f));
	_colButton = new ToggleButton(Vector2(50.0f, 200.0f), Vector2(200.0f, 100.0f), Vector4(0.1f, 0.2f, 1.0f, 0.8f));
	_posButton = new ToggleButton(Vector2(50.0f, 350.0f), Vector2(200.0f, 100.0f), Vector4(0.1f, 0.2f, 1.0f, 0.8f));

	_menuGui->AddButton(_octButton);
	_menuGui->AddButton(_colButton);
	_menuGui->AddButton(_posButton);
	_octree.SetBoundingArea(BoundingBox(Vector3::zeroes, Vector3(16.0f * Chunk::dimension)));

	_renderer.SetAmbientLight(Vector3(1.0f, 1.0f, 1.0f), 0.5f);
	_renderer.SetGlobalLightDir(Vector3(2.0f, 1.0f, 0.5f));
}

void
VEngine::Destroy()
{
	DestroyWorld();
	DestroyFileManagers();
	DestroyResourceManagers();
}

void
VEngine::DestroyWorld()
{
	delete _menuGui;
	delete _world;
}

void
VEngine::DestroyFileManagers()
{
	delete shaderFileManager.GetSingletonPointer();
}
void
VEngine::DestroyResourceManagers()
{
	delete pipelineManager.GetSingletonPointer();
	delete programManager.GetSingletonPointer();
	delete shaderManager.GetSingletonPointer();
	delete textureManager.GetSingletonPointer();
	delete meshManager.GetSingletonPointer();
	delete voxelArrayManager.GetSingletonPointer();
	delete gameObjectManager.GetSingletonPointer();
}
void
VEngine::DestroyOtherManagers()
{

}

void
VEngine::DestroyGLFWLibrary()
{
	Window::Destroy();
	glfwTerminate();
}

Renderer& 
VEngine::GetRenderer()
{
	return _renderer;
}

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	Vector2 Size;       // Size of glyph
	Vector2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

void
VEngine::Run()
{
	_world->Init();
	_octree.UpdateTree();
	_octree.Update();

	Time::Update();
	while (Window::IsOpened()) {
		Time::Update();
		Input::UpdateMouseOffset();

		if (Input::GetCursorMode()) {
			if (_menuGui->Update()) {
				_debugConfig.drawOctree = _octButton->GetValue();
				_debugConfig.drawColliders = _colButton->GetValue();
				_debugConfig.drawPositions = _posButton->GetValue();
			}
		}

		_world->Update();
		_world->Physic();
		_octree.UpdateTree();
		_octree.Update();

		_world->LateUpdate();

		GameObject::HandleDestroyed();

		Draw();
		_octree.Draw(&_renderer);
		_world->Draw(&_renderer);
		_world->LateDraw(&_renderer);

#ifdef VE_DEBUG
		if (_debugConfig.drawOctree) {
			_octree.DrawDebug(&_renderer);
		}
#endif
		if(Input::GetCursorMode())
			_menuGui->Draw(&_renderer);
		
		Input::UpdateInput();
		Window::HandleWindow();

		std::cout << "\rFPS: " << 1.0f / Time::DeltaTime() << "\tPosition: " << _renderer.GetActiveCamera()->GetPosition();

		if (Input::IsPressed("Exit")) {
			Window::Close();
		}
	}
}

void
VEngine::Draw()
{
	_renderer.ClearBuffers();
	_renderer.UpdateProjection();
	_renderer.UpdateView();
}

void
VEngine::ErrorHandler(int error, const char* description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

}