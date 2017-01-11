#include "Renderer.h"

namespace vengine {
int Renderer::_rendererNumber;

void
Renderer::Init()
{
	for (int i = 0; i < _shadersNumber; ++i) {
		_vertShaders[i] = 0;
		_fragShaders[i] = 0;
		_shaderModes[i] = 0;
	}

	_activeCamera = NULL;
	_clearColor = { 1.0f, 1.0f, 1.0f };
	_pipe = pipelineManager.GetPipeline("renderer" + std::to_string(++_rendererNumber));

	glPointSize(40.0f);
}

void
Renderer::Delete()
{
	pipelineManager.DeletePipeline(_pipe);
	_pipe = 0;
	_activeCamera = NULL;
}

int
Renderer::PrepareShader(const std::string& name, const std::string& path, Shader::ShaderType type)
{
	unsigned int shader = shaderManager.GetShader(name, type);
	int error = shaderManager.LoadShaderDataFromFile(shader, path);
	if (error)
		return error;

	if (error = shaderManager.CompileShader(shader)) {
		const char* logs = shaderManager.GetCompileLog(shader);
		std::cout << "Compilation failed for: " << name << ", file: " << path << std::endl;
		std::cout << logs << std::endl;
		return error;
	}

	unsigned int program = programManager.GetProgram(name);
	programManager.SetProgramSeparable(program, true);
	programManager.AttachShader(program, shader);

	if (error = programManager.LinkProgram(program)) {
		const char* logs = programManager.GetLinkLog(program);
		std::cout << "Linking failed for: " << name << std::endl;
		std::cout << logs << std::endl;
		return error;
	}

	shaderManager.DeleteShader(shader);

	return 0;
}


int
Renderer::AddShader(const std::string& name, const std::string& path, Shader::ShaderType type,
					ShadersIndexes destination)
{
	int error = PrepareShader(name, path, type);
	if (error)
		return error;

	unsigned int program = programManager.GetProgram(name);

	switch (type) {
	case Shader::VERTEX:
		_vertShaders[destination] = program;
		break;
	case Shader::FRAGMENT:
		_fragShaders[destination] = program;
		break;
	default:
		return VE_EVAL;
	}

	return 0;
}

int
Renderer::AddShader(const std::string& name, Shader::ShaderType type, ShadersIndexes destination)
{
	unsigned int program = programManager.GetProgram(name);

	switch (type) {
	case Shader::VERTEX:
		_vertShaders[destination] = program;
		break;
	case Shader::FRAGMENT:
		_fragShaders[destination] = program;
		break;
	default:
		return VE_EVAL;
	}

	return 0;
}


void
Renderer::SetAmbientLight(const Vector3& color, float strength)
{
	if (_ambStrength != strength) {
		for (int i = 0; i < _shadersNumber; ++i)
			programManager.SetUniform(_fragShaders[i], "ambientStrength", GlProgram::FRAGMENT, strength);
		_ambStrength = strength;
	}

	Vector3 normColor = Vector3::Normalized(color);
	if (_ambColor != normColor) {
		for (int i = 0; i < _shadersNumber; ++i)
			programManager.SetUniform(_fragShaders[i], "ambientLightColor", GlProgram::FRAGMENT, normColor);
		_ambColor = normColor;
	}
}

void 
Renderer::SetGlobalLightDir(const Vector3& dir)
{
	Vector3 normDir = Vector3::Normalized(dir);
	if (_lightDir != normDir) {
		for (int i = 0; i < _shadersNumber; ++i)
			programManager.SetUniform(_fragShaders[i], "globalLightDir", GlProgram::FRAGMENT, normDir);
		_lightDir = normDir;
	}
}

Renderer& 
Renderer::SetModelMatrix(const Matrix4& model)
{
	if (model != _modelMatrix) {
		for (int i = 0; i < _shadersNumber; ++i)
			programManager.SetUniform(_vertShaders[i], "model", GlProgram::VERTEX, model);
		_modelMatrix = model;
	}

	return *this;
}

const Matrix4& 
Renderer::GetProjectionMatrix() const
{
	return _projMatrix;
}

const Matrix4& 
Renderer::GetModelMatrix() const
{
	return _modelMatrix;
}

void 
Renderer::SetWireColor(const Vector3& color)
{
	if (color != _wireColor) {
		for (int i = 0; i < _shadersNumber; ++i) 
			programManager.SetUniform(_fragShaders[i], "wireColor", GlProgram::FRAGMENT, color);
	}
}

void
Renderer::SetClearColor(const Vector3& color)
{
	_clearColor = color;
}

void
Renderer::SetAtlasTileSize(float tileSize)
{
	if (_tileSize != tileSize) {
		programManager.SetUniform(_fragShaders[VOXEL], "tileSize", GlProgram::VERTEX, tileSize);
		_tileSize = tileSize;
	}
}

void
Renderer::SetTexture(unsigned int tex)
{
	if (_tex != tex) {
		textureManager.BindTexture(tex);
		_tex = tex;
	}
}

void
Renderer::ClearBuffers()
{
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
Renderer::DepthTestEnable()
{
	glEnable(GL_DEPTH_TEST);
}

void
Renderer::DepthTestDisable()
{
	glDisable(GL_DEPTH_TEST);
}


void 
Renderer::Draw(const RenderInfo& info, ShadersIndexes mode)
{
	if (mode != GUI) {
		if (info.wired) {
			if (!IsEnabled(WIRED, mode)) {
				programManager.SetUniform(_fragShaders[mode], "wired", GlProgram::FRAGMENT, 1);
				ToggleState(WIRED, mode);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			SetWireColor(info.color);
		}
		else {
			if (IsEnabled(WIRED, mode)) {
				programManager.SetUniform(_fragShaders[mode], "wired", GlProgram::FRAGMENT, 0);
				ToggleState(WIRED, mode);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			if (info.textured) {
				if (!IsEnabled(WIRED, mode)) {
					programManager.SetUniform(_fragShaders[mode], "textured", GlProgram::FRAGMENT, 1);
					ToggleState(TEXTURED, mode);
				}
				SetTexture(info.tex);
			}
			else {
				if (IsEnabled(WIRED, mode)) {
					programManager.SetUniform(_fragShaders[mode], "textured", GlProgram::FRAGMENT, 0);
					ToggleState(TEXTURED, mode);
				}
			}
		}
	}


	if (pipelineManager.GetProgram(_pipe, GlProgram::VERTEX) != _vertShaders[mode])
		pipelineManager.ChangeStage(_pipe, _vertShaders[mode], GlProgram::VERTEX);
	if (pipelineManager.GetProgram(_pipe, GlProgram::FRAGMENT) != _fragShaders[mode])
		pipelineManager.ChangeStage(_pipe, _fragShaders[mode], GlProgram::FRAGMENT);


	pipelineManager.BindPipeline(_pipe);
	glDrawElements(info.drawType, info.indicesNumber, GL_UNSIGNED_INT, 0);

}


void
Renderer::UpdateProjection()
{
	assert(_activeCamera != nullptr, "Invalid camera");
	if (Window::SizeChanged() || _activeCamera->PerspectiveChanged()) {
		int w, h;
		Window::GetWindowSize(&w, &h);
		_activeCamera->GetProjectionMatrix(&_projMatrix, w, h);

		glViewport(0, 0, w, h);
		for (int i = 0; i < _shadersNumber; ++i)
			programManager.SetUniform(_vertShaders[i], "projection", GlProgram::VERTEX, _projMatrix);

		programManager.SetUniform(_vertShaders[GUI], "projection", GlProgram::VERTEX, Matrix4::GetOrtho(0.0f, (float)w, (float)h, 0.0f, -1.0f, 1.0f));
	}
}

void 
Renderer::UpdateView()
{
	assert(_activeCamera != nullptr, "Invalid camera");


	if (_activeCamera->OrientationChanged()) {
		_viewMatrix = _activeCamera->GetViewMatrix();

		for (int i = 0; i < _shadersNumber; ++i)
			programManager.SetUniform(_vertShaders[i], "view", GlProgram::VERTEX, _viewMatrix);
	}
}

Renderer& 
Renderer::SetActiveCamera(CameraFPP* camera)
{
	_activeCamera = camera;

	return *this;
}

CameraFPP*
Renderer::GetActiveCamera() const
{
	return _activeCamera;
}

}