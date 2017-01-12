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

	_activeCamera = nullptr;
	_clearColor = { 1.0f, 1.0f, 1.0f };
	_pipe = pipelineManager.GetPipeline("renderer" + std::to_string(++_rendererNumber));

	glPointSize(40.0f);
}

void
Renderer::Delete()
{
	pipelineManager.DeletePipeline(_pipe);
	_pipe = 0;
	_activeCamera = nullptr;
}

int
Renderer::PrepareShader(const std::string& name, const std::string& path, Shader::ShaderType type)
{
	/* Load shader */
	unsigned int shader = shaderManager.GetShader(name, type);
	int error = shaderManager.LoadShaderDataFromFile(shader, path);
	if (error)
		return error;

	/* Compile shader */
	if (error = shaderManager.CompileShader(shader)) {
		const char* logs = shaderManager.GetCompileLog(shader);
		std::cout << "Compilation failed for: " << name << ", file: " << path << std::endl;
		std::cout << logs << std::endl;
		return error;
	}

	/* Attach it to program */
	unsigned int program = programManager.GetProgram(name);
	programManager.SetProgramSeparable(program, true);
	programManager.AttachShader(program, shader);

	/* Compile program */
	if (error = programManager.LinkProgram(program)) {
		const char* logs = programManager.GetLinkLog(program);
		std::cout << "Linking failed for: " << name << std::endl;
		std::cout << logs << std::endl;
		return error;
	}

	/* Shader will not be needed, so we can delete it */
	shaderManager.DeleteShader(shader);

	return 0;
}


int
Renderer::AddShader(const std::string& name, const std::string& path, Shader::ShaderType type,
					ShadersIndexes destination)
{
	/* First compile it into the program */
	int error = PrepareShader(name, path, type);
	if (error)
		return error;

	/* Get this program */
	unsigned int program = programManager.GetProgram(name);

	/* And add it as proper type shader */
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
	/* We are adding existing program, we do not need to compile it */
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
	/* Only for not gui shaders */
	if (_ambStrength != strength) {
		for (int i = 0; i < GUI; ++i)
			programManager.SetUniform(_fragShaders[i], "ambientStrength", strength);
		_ambStrength = strength;
	}

	/* Only for not gui shaders */
	Vector3 normColor = Vector3::Normalized(color);
	if (_ambColor != normColor) {
		for (int i = 0; i < GUI; ++i)
			programManager.SetUniform(_fragShaders[i], "ambientLightColor", normColor);
		_ambColor = normColor;
	}
}

void 
Renderer::SetGlobalLightDir(const Vector3& dir)
{
	/* Only for not gui shaders */
	Vector3 normDir = Vector3::Normalized(dir);
	if (_lightDir != normDir) {
		for (int i = 0; i < GUI; ++i)
			programManager.SetUniform(_fragShaders[i], "globalLightDir", normDir);
		_lightDir = normDir;
	}
}

Renderer& 
Renderer::SetModelMatrix(const Matrix4& model)
{
	/* Only for not gui shaders */
	if (model != _modelMatrix) {
		for (int i = 0; i < GUI; ++i)
			programManager.SetUniform(_vertShaders[i], "model", model);
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
	/* Only for not gui shaders */
	if (color != _wireColor) {
		for (int i = 0; i < GUI; ++i)
			programManager.SetUniform(_fragShaders[i], "wireColor", color);
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
		programManager.SetUniform(_fragShaders[VOXEL], "tileSize", tileSize);
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
	/* Default inital values */
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
	/* If not gui */
	if (mode != GUI) {
		/* If wired, we must inform shader about that */
		if (info.wired) {
			/* If not wired, change uniform*/
			if (!IsEnabled(WIRED, mode)) {
				programManager.SetUniform(_fragShaders[mode], "wired", 1);
				ToggleState(WIRED, mode);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			SetWireColor(info.color);
		}
		else {
			/* If shader is wired, turn it off, as it have highest priority */
			if (IsEnabled(WIRED, mode)) {
				programManager.SetUniform(_fragShaders[mode], "wired", 0);
				ToggleState(WIRED, mode);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			/* Same as for wired*/
			if (info.textured) {
				if (!IsEnabled(WIRED, mode)) {
					programManager.SetUniform(_fragShaders[mode], "textured", 1);
					ToggleState(TEXTURED, mode);
				}
				SetTexture(info.tex);
			}
			else {
				/* Indicate that we want to draw color */
				if (IsEnabled(WIRED, mode)) {
					programManager.SetUniform(_fragShaders[mode], "textured", 0);
					ToggleState(TEXTURED, mode);
				}
			}
		}
	}

	/* Bind proper shaders if not already loaded */
	if (pipelineManager.GetProgram(_pipe, GlProgram::VERTEX) != _vertShaders[mode])
		pipelineManager.ChangeStage(_pipe, _vertShaders[mode], GlProgram::VERTEX);
	if (pipelineManager.GetProgram(_pipe, GlProgram::FRAGMENT) != _fragShaders[mode])
		pipelineManager.ChangeStage(_pipe, _fragShaders[mode], GlProgram::FRAGMENT);

	/* And bind pipeline just in case there is other renderer */
	pipelineManager.BindPipeline(_pipe);
	/* Just draw elements */
	glDrawElements(info.drawType, info.indicesNumber, GL_UNSIGNED_INT, 0);
}

void
Renderer::UpdateProjection()
{
	assert(_activeCamera != nullptr, "Invalid camera");
	/* Update projection if window has changed or camera projection */
	if (Window::SizeChanged() || _activeCamera->PerspectiveChanged()) {
		int w, h;
		Window::GetWindowSize(&w, &h);
		_activeCamera->GetProjectionMatrix(&_projMatrix, w, h);

		glViewport(0, 0, w, h);
		for (int i = 0; i < GUI; ++i)
			programManager.SetUniform(_vertShaders[i], "projection", _projMatrix);

		/* For GUI we want to set special projection */
		programManager.SetUniform(_vertShaders[GUI], "projection", Matrix4::GetOrtho(0.0f, (float)w, (float)h, 0.0f, -1.0f, 1.0f));
	}
}

void 
Renderer::UpdateView()
{
	assert(_activeCamera != nullptr, "Invalid camera");

	/* Update only if camera orientation has changed */
	if (_activeCamera->OrientationChanged()) {
		_viewMatrix = _activeCamera->GetViewMatrix();

		for (int i = 0; i < GUI; ++i)
			programManager.SetUniform(_vertShaders[i], "view", _viewMatrix);
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