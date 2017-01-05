#pragma once

#include "VEMath.h"

#include "Managers/ShaderManager.h"
#include "Managers/GlProgramManager.h"
#include "Managers/GlPipelineManager.h"
#include "Managers/TextureManager.h"
#include "RenderInfo.h"
#include "CameraFPP.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <iostream>
namespace vengine {

class Renderer {
public:
	enum ShadersIndexes {
		STANDARD = 0,
		VOXEL = 1
	};

	void Init();
	void Delete();

	int AddShader(const std::string& name, const std::string& path, Shader::ShaderType type, ShadersIndexes destination);
	int AddShader(const std::string& name, Shader::ShaderType type, ShadersIndexes destination);

	Renderer& SetModelMatrix(const Matrix4& model);

	const Matrix4& GetProjectionMatrix() const;
	const Matrix4& GetModelMatrix() const;

	void SetClearColor(const Vector3& color);

	void SetAtlasTileSize(float tileSize);

	void SetAmbientLight(const Vector3& color, float strength);
	void SetGlobalLightDir(const Vector3& dir);

	void UpdateView();
	void UpdateProjection();
	Renderer& SetActiveCamera(CameraFPP* camera);
	CameraFPP* GetActiveCamera() const;

	void ClearBuffers();

	void DepthTestEnable();

	void DepthTestDisable();

	void Draw(const RenderInfo& info, ShadersIndexes mode);

private:


	enum ShadersModes {
		TEXTURED = 0x01u,
		WIRED = 0x02u
	};


	static const int _shadersNumber = 2;
	static int _rendererNumber;

	unsigned int _pipe;

	unsigned int _shaderModes[_shadersNumber];
	unsigned int _vertShaders[_shadersNumber];
	unsigned int _fragShaders[_shadersNumber];

	CameraFPP* _activeCamera;


	Vector3 _lightDir;
	Vector3 _ambColor;
	float _ambStrength;

	Vector3 _clearColor;
	Vector3 _wireColor;
	unsigned int _tex;

	float _tileSize;

	Matrix4 _modelMatrix;
	Matrix4 _viewMatrix;
	Matrix4 _projMatrix;

	int PrepareShader(const std::string& name, const std::string& path, Shader::ShaderType type);
	void SetWireColor(const Vector3& color);
	void SetTexture(unsigned int tex);
	inline bool IsEnabled(ShadersModes mode, ShadersIndexes index);
	inline void ToggleState(ShadersModes mode, ShadersIndexes index);

};

inline bool
Renderer::IsEnabled(ShadersModes mode, ShadersIndexes index)
{
	return (_shaderModes[index] & mode) != 0;
}

inline void
Renderer::ToggleState(ShadersModes mode, ShadersIndexes index)
{
	_shaderModes[index] ^= (1 << (mode - 1));
}


}