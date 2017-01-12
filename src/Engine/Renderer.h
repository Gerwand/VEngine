#pragma once

#include "VEMath.h"

#include "Resources/Managers/ShaderManager.h"
#include "Resources/Managers/GlProgramManager.h"
#include "Resources/Managers/GlPipelineManager.h"
#include "Resources/Managers/TextureManager.h"
#include "RenderInfo.h"
#include "CameraFPP.h"
#include "IO/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <iostream>

namespace vengine {

/*
* Renderer class is used for rendering vertices binded by Renderable resources, and for managing
* uniforms and resources. Can use multiple shaders for rendering. Currently Standard, Voxel and GUI
*/
class Renderer {
public:
	enum ShadersIndexes {
		STANDARD,	/* Used for rendering standart objects */
		VOXEL,		/* used for rendering obejcts created from voxels */
		GUI			/* Used for rendering GUI */
	};

	/* Initializes resources, prepares pipeline */
	void Init();
	/* Deletes all used resources */
	void Delete();

	/*
	* Loads shader from the file and adds both: loads it to the programManager and loads it as shader of given type.
	* Additionaly, it must specified what type of shader it is - vertex or fragment
	*/
	int AddShader(const std::string& name, const std::string& path, Shader::ShaderType type, ShadersIndexes destination);
	/* Same as above function, however it loads shader from shader manager */
	int AddShader(const std::string& name, Shader::ShaderType type, ShadersIndexes destination);

	/* Set model matrix for next draw for STANDARD and VOXEL shaders */
	Renderer& SetModelMatrix(const Matrix4& model);

	const Matrix4& GetProjectionMatrix() const;
	const Matrix4& GetModelMatrix() const;

	void SetClearColor(const Vector3& color);

	/*
	* Depending on how many tiles are stored in one row of the atlas texture, this should be set in normalized values.
	* If there are 16 tiles on each rowe, it must be 1/16. It is set in Voxel shader.
	*/
	void SetAtlasTileSize(float tileSize);

	/* Set ambient light color and strength. Can be used with direction for stering night and day */
	void SetAmbientLight(const Vector3& color, float strength);
	/* Set global light direction */
	void SetGlobalLightDir(const Vector3& dir);

	/* Update view matrix basing on active camera for STANDARD and VOXEL shaders */
	void UpdateView();
	/* Update projection matrix basing on active camera for all shaders */
	void UpdateProjection();

	Renderer& SetActiveCamera(CameraFPP* camera);
	CameraFPP* GetActiveCamera() const;

	/* Set default values for next draw and clear buffers */
	void ClearBuffers();

	void DepthTestEnable();
	void DepthTestDisable();

	/* Draw earlier binded vertices using given shader. */
	void Draw(const RenderInfo& info, ShadersIndexes mode);

private:
	/* Most of the variables are used for remembering current state of the uniforms to reduce some cost of sending data to GPU */

	/* Used for indicating in which mode the shader is drawing */
	enum ShadersModes {
		TEXTURED = 0x01u,	/* Solid or texture mode */
		WIRED = 0x02u		/* Wired mode */
	};

	static const int _shadersNumber = 3;	/* Number of types in class */
	static int _rendererNumber;				/* Indicates number of renderers currently active. However, class has not been optimized
											 * for using multiple renderers if they are sharing shaders!  It can cause unpredictable results. */

	unsigned int _pipe;		/* Handle for pipeline object in pipelineManager */

	unsigned int _shaderModes[_shadersNumber]; /* Keeps information about current mode of the binded shader*/
	unsigned int _vertShaders[_shadersNumber]; /* Keeps handles for the vertex shaders */
	unsigned int _fragShaders[_shadersNumber]; /* Keeps handles for the fragment shaders */

	CameraFPP* _activeCamera; /* Pointer to the active camera for rendering */


	Vector3 _lightDir;	/* Last used global light direction */
	Vector3 _ambColor;	/* Last used ambient light color */
	float _ambStrength; /* Last used ambient light strength */

	Vector3 _clearColor;	/* Buffer will be cleared with this color every frame */
	Vector3 _wireColor;		/* Color of the drawn wires */
	unsigned int _tex;		/* Currently binded texture */

	float _tileSize;	/* Atlas tile size in normalized UV coordinates */

	Matrix4 _modelMatrix;	/* Last used model matrix */
	Matrix4 _viewMatrix;	/* Last used view matrix */
	Matrix4 _projMatrix;	/* Last used projection matrix */

	/*
	* Loads shader from file, compiles its, attaches to new program and links. It will be saved in programManager with given name
	*
	* @return error code - 0 if succeed
	*/
	int PrepareShader(const std::string& name, const std::string& path, Shader::ShaderType type);

	/* This functions should not be called by the developer. Texture and wire color should be passed through RenderInfo structure. */
	/* Set color of the wires */
	void SetWireColor(const Vector3& color);
	/* Set active texture handle */
	void SetTexture(unsigned int tex);

	/* Check if shader is working in given mode (textured or wired) */
	inline bool IsEnabled(ShadersModes mode, ShadersIndexes index);
	/* Toggle state on/off */
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