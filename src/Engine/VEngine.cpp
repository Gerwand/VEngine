#include "VEngine.h"

namespace vengine {

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
		printf("Cannot initialize Window. Error code: %d", error);
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
#ifdef VE_DEBUG
	_menuGui = new Canvas(Vector4(0.0f, 0.0f, 0.0f, 0.7f));
	GameObject::debugConfig = &_debugConfig;
	_debugConfig.drawColliders = false;
	_debugConfig.drawOctree = false;
	_debugConfig.drawPositions = false;
#endif
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
	/* Create meshes and VA of prototypes objects */
	VoxelMesh* mesh = new VoxelMesh;
	mesh->Init("Sword");
	int vaSword = voxelArrayManager.GetVoxelArray("VoxelSword");
	voxelArrayManager.SetDimension(vaSword, swordDim.x, swordDim.y, swordDim.z);
	voxelArrayManager.SetVoxels(vaSword, swordVoxels);
	voxelArrayManager.SetVoxelSize(vaSword, swordSize);
	voxelArrayManager.GenerateMesh(vaSword, mesh);
	unsigned int swordMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("Pickaxe");
	int vaPick = voxelArrayManager.GetVoxelArray("Pickaxe");
	voxelArrayManager.SetDimension(vaPick, pickDim.x, pickDim.y, pickDim.z);
	voxelArrayManager.SetVoxels(vaPick, pickVoxels);
	voxelArrayManager.SetVoxelSize(vaPick, pickSize);
	voxelArrayManager.GenerateMesh(vaPick, mesh);
	unsigned int pickMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("Dirt");
	int dirtVox = voxelArrayManager.GetVoxelArray("Dirt");
	voxelArrayManager.SetDimension(dirtVox, cubeDim.x, cubeDim.y, cubeDim.z);
	voxelArrayManager.SetVoxels(dirtVox, dirt);
	voxelArrayManager.SetVoxelSize(dirtVox, cubeSize);
	voxelArrayManager.GenerateMesh(dirtVox, mesh);
	unsigned int dirtMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("Grass");
	int grassVox = voxelArrayManager.GetVoxelArray("Grass");
	voxelArrayManager.SetDimension(grassVox, cubeDim.x, cubeDim.y, cubeDim.z);
	voxelArrayManager.SetVoxels(grassVox, grass);
	voxelArrayManager.SetVoxelSize(grassVox, cubeSize);
	voxelArrayManager.GenerateMesh(grassVox, mesh);
	unsigned int grassMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("Wood");
	int woodVox = voxelArrayManager.GetVoxelArray("Wood");
	voxelArrayManager.SetDimension(woodVox, cubeDim.x, cubeDim.y, cubeDim.z);
	voxelArrayManager.SetVoxels(woodVox, wood);
	voxelArrayManager.SetVoxelSize(woodVox, cubeSize);
	voxelArrayManager.GenerateMesh(woodVox, mesh);
	unsigned int woodMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("Stone");
	int stoneVox = voxelArrayManager.GetVoxelArray("Stone");
	voxelArrayManager.SetDimension(stoneVox, cubeDim.x, cubeDim.y, cubeDim.z);
	voxelArrayManager.SetVoxels(stoneVox, stone);
	voxelArrayManager.SetVoxelSize(stoneVox, cubeSize);
	voxelArrayManager.GenerateMesh(stoneVox, mesh);
	unsigned int stoneMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("Arm");
	int armVox = voxelArrayManager.GetVoxelArray("Arm");
	voxelArrayManager.SetDimension(armVox, armDim.x, armDim.y, armDim.z);
	voxelArrayManager.SetVoxels(armVox, armVoxels);
	voxelArrayManager.SetVoxelSize(armVox, armSize);
	voxelArrayManager.GenerateMesh(armVox, mesh);
	unsigned int armMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("Skeleton");
	int skelVox = voxelArrayManager.GetVoxelArray("Skeleton");
	voxelArrayManager.SetDimension(skelVox, skelDim.x, skelDim.y, skelDim.z);
	voxelArrayManager.SetVoxels(skelVox, skelVoxels);
	voxelArrayManager.SetVoxelSize(skelVox, skelSize);
	voxelArrayManager.GenerateMesh(skelVox, mesh);
	unsigned int skelMesh = meshManager.AddMesh(mesh);

	mesh = new VoxelMesh;
	mesh->Init("SkeletonHead");
	int skelHeadVox = voxelArrayManager.GetVoxelArray("SkeletonHead");
	voxelArrayManager.SetDimension(skelHeadVox, cubeDim.x, cubeDim.y, cubeDim.z);
	voxelArrayManager.SetVoxels(skelHeadVox, skeletonHead);
	voxelArrayManager.SetVoxelSize(skelHeadVox, skelHeadSize);
	voxelArrayManager.GenerateMesh(skelHeadVox, mesh);
	unsigned int skelHeadMesh = meshManager.AddMesh(mesh);

	/* Objects initialization */
	Transform transformMain;

	/* Sword mesh */
	transformMain.SetPosition(Vector3(0.075f, 0.1f, 0.0f));
	MeshedObject* swordObject = new MeshedObject("Sword");
	swordObject->SetMesh(swordMesh);
	swordObject->SetTransform(transformMain);

	/* Projectile */
	transformMain.SetPosition(Vector3(0.075f, 0.1f, 0.0f));
	Projectile* projectile = new Projectile("Projectile");
	projectile->SetMesh(swordMesh);
	projectile->SetTransform(transformMain);
	projectile->SetCollider(BoundingBox(Vector3::zeroes, Vector3(0.1f)));

	/* Pickaxe mesh */
	transformMain.SetPosition(Vector3(0.075f, 0.09f, 0.0f));
	MeshedObject* pickObject = new MeshedObject("Pickaxe");
	pickObject->SetMesh(pickMesh);
	pickObject->SetTransform(transformMain);

	/* Dirt mesh */
	transformMain.SetPosition(Vector3(0.075f, 0.05f, 0.0f));
	transformMain.SetRotation(Quaternion().SetRotationX(45.0f).SetRotationY(45.0f));
	MeshedObject* dirtObject = new MeshedObject("Dirt");
	dirtObject->SetMesh(dirtMesh);
	dirtObject->SetTransform(transformMain);

	/* Grass mesh */
	MeshedObject* grassObject = new MeshedObject("Grass");
	grassObject->SetMesh(grassMesh);
	grassObject->SetTransform(transformMain);

	/* Wood mesh */
	MeshedObject* woodObject = new MeshedObject("Wood");
	woodObject->SetMesh(woodMesh);
	woodObject->SetTransform(transformMain);

	/* Stone mesh */
	MeshedObject* stoneObject = new MeshedObject("Stone");
	stoneObject->SetMesh(stoneMesh);
	stoneObject->SetTransform(transformMain);

	/* Player arm mesh */
	transformMain.SetPosition(Vector3(0.27f, -0.15f, 0.1f));
	transformMain.SetRotation(Quaternion().SetRotationZ(30.0f));
	MeshedObject* armObject = new MeshedObject("Arm");
	armObject->SetMesh(armMesh);
	armObject->SetTransform(transformMain);

	/* Enemy */
	transformMain.SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	transformMain.SetRotation(Quaternion::identity);
	Enemy* enemyObject = new Enemy("Enemy");
	enemyObject->SetMesh(skelMesh);
	enemyObject->SetTransform(transformMain);
	enemyObject->SetCollider(BoundingBox(transformMain.GetPosition(),
										 Vector3((float)skelDim.x, (float)skelDim.y, (float)skelDim.z) * skelSize));

	/*  Enemy's Head */
	transformMain.SetPosition(Vector3(0.0f, 0.90f, -0.2f));
	transformMain.SetRotation(Quaternion::identity);
	EnemyHead* enemyHead = new EnemyHead("EnemyHead");
	enemyHead->SetMesh(skelHeadMesh);
	enemyHead->SetTransform(transformMain);
	enemyHead->AttachTo(enemyObject);

	/* Add objects to manager */
	gameObjectManager.AddGameObject(swordObject);
	gameObjectManager.AddGameObject(projectile);
	gameObjectManager.AddGameObject(pickObject);
	gameObjectManager.AddGameObject(dirtObject);
	gameObjectManager.AddGameObject(grassObject);
	gameObjectManager.AddGameObject(woodObject);
	gameObjectManager.AddGameObject(stoneObject);
	gameObjectManager.AddGameObject(armObject);
	gameObjectManager.AddGameObject(enemyObject);
}

void 
VEngine::LoadWorld()
{
	/* Initialize renderer*/
	CameraFPP* camera = new CameraFPP;
	_renderer.SetClearColor(Vector3(135 / 255.0f, 191 / 255.0f, 255 / 255.0f));
	_renderer.SetActiveCamera(camera);
	_renderer.SetAmbientLight(Vector3(1.0f, 1.0f, 1.0f), 0.5f);
	_renderer.SetGlobalLightDir(Vector3(2.0f, 1.0f, 0.5f));

	/* Create world */
	_world = new World("World");
	_world->Rename("World");

	/* Initialize player */
	Transform transformPlayer;
	transformPlayer.SetPosition(Vector3(4.0f, 50.0f, 4.0f));
	PlayerController* player = new PlayerController;
	player->SetCamera(camera);
	player->SetTag("Player");
	player->GetCollider().SetDimension(Vector3(0.5f, 1.5f, 0.5f));
	player->AttachTo(_world);
	player->SetTransform(transformPlayer);
	player->SetOctree(&_octree);

	/* And player's head */
	PlayerHead* playerHead = new PlayerHead;
	playerHead->SetCamera(camera);
	playerHead->SetTag("PlayerHead");
	playerHead->SetTransform(Transform().SetPosition(Vector3(0.0f, 0.6f, 0.0f)));
	playerHead->AttachTo(player);
	
	/* Add arm */
	MeshedObject* arm = (MeshedObject *)gameObjectManager.Instantiate("Arm");
	arm->AttachTo(playerHead);
	player->SetArm(arm);

	/* Spawn one enemy */
	Enemy* enemyObject = (Enemy *)gameObjectManager.Instantiate("Enemy");
	enemyObject->AttachTo(_world);
	enemyObject->SetPlayer(player);
	EnemyHead* eHead = (EnemyHead *)(enemyObject->GetChild());
	eHead->SetPlayer(player);

	/* Generate terrain */
	TerrainGenerator terrainGen(0, 5, 1);
	terrainGen.SetSmoothness(256);
	terrainGen.SetDetails(1);
	terrainGen.SetSpread(32);
	terrainGen.SetSeed(312538u);
	for (int z = 0; z < 16; ++z) {
		for (int y = 0; y < 16; ++y) {
			for (int x = 0; x < 16; ++x) {
				Chunk* chunk = new Chunk(Vector3((x - 8.0f) * Chunk::dimension, (y - 8.0f) * Chunk::dimension, (z - 8.0f) * Chunk::dimension));
				if(terrainGen.GetChunk(chunk))
					_octree.Add(chunk);
			}
		}
	}


#ifdef VE_DEBUG
	/* Add GUI */
	_octButton = new ToggleButton(Vector2(50.0f, 50.0f), Vector2(200.0f, 100.0f), Vector4(1.0f, 0.0f, 0.0f, 0.8f));
	_colButton = new ToggleButton(Vector2(50.0f, 200.0f), Vector2(200.0f, 100.0f), Vector4(0.0f, 0.0f, 1.0f, 0.8f));
	_posButton = new ToggleButton(Vector2(50.0f, 350.0f), Vector2(200.0f, 100.0f), Vector4(1.0f, 1.0f, 0.0f, 0.8f));
	_wirButton = new ToggleButton(Vector2(50.0f, 500.0f), Vector2(200.0f, 100.0f), Vector4(0.0f, 1.0f, 0.0f, 0.8f));

	_menuGui->AddButton(_octButton);
	_menuGui->AddButton(_colButton);
	_menuGui->AddButton(_posButton);
	_menuGui->AddButton(_wirButton);
#endif

	/* Initialize Octree */
	_octree.Add(player);
	_octree.Add(enemyObject);
	_octree.SetBoundingArea(BoundingBox(Vector3::zeroes, Vector3(16.0f * Chunk::dimension)));
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

void
VEngine::Run()
{
	/* Initialize objects */
	_world->Init();

	/* Initialize octree */
	_octree.UpdateTree();
	_octree.Update();

	Time::Update();

	/* Main loop */
	while (Window::IsOpened()) {
		Time::Update();
		Input::UpdateMouseOffset();

#ifdef VE_DEBUG
		if (Input::GetCursorMode()) {
			/* Update menu options only if cursor is active */
			if (_menuGui->Update()) {
				_debugConfig.drawOctree = _octButton->GetValue();
				_debugConfig.drawColliders = _colButton->GetValue();
				_debugConfig.drawPositions = _posButton->GetValue();
				Mesh::SetWired(_wirButton->GetValue());
			}
		}
#endif

		_world->Update();
		_world->Physic();
		_octree.UpdateTree();
		_octree.Update();
		
		/* Late update is done after update, physic and collision checks */
		_world->LateUpdate();

		/* Destroy are destroyed object */
		GameObject::HandleDestroyed();

		/* Prepare for drawing */
		Draw();

		/* Draw chunks */
		_octree.Draw(&_renderer);
		/* Draw world */
		_world->Draw(&_renderer);
		_world->LateDraw(&_renderer);

#ifdef VE_DEBUG
		if (_debugConfig.drawOctree) {
			_octree.DrawDebug(&_renderer);
		}
		/* Draw menu only if cursor is active */
		if(Input::GetCursorMode())
			_menuGui->Draw(&_renderer);
#endif	

		Input::UpdateInput();
		Window::HandleWindow();

#ifdef VE_DEBUG
		/* Fps and position in console */
		std::cout << "\rFPS: " << 1.0f / Time::DeltaTime() << "\tPosition:" << _renderer.GetActiveCamera()->GetPosition();
#endif

		/* If ESC is pressed, close */
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