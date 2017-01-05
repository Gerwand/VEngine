#pragma once

#include "Vertex.h"
#include "Managers/GlBuffer.h"
#include "Managers/VertexArray.h"
#include "Managers/TextureManager.h"
#include "Renderable.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>



namespace vengine {


class Mesh : public Renderable 
{
public:
	void Init(const std::string& name);


	void SetTextured(bool textured);
	void SetTexture(unsigned int tex);
	void SetTexture(const std::string& name);
	unsigned int GetTexture() const;
	const std::string& GetName() const;

	void Draw(Renderer* renderer);

	static void SetWired(bool wired);
	static void SetWireColor(const Vector3& color);
protected:
	std::string _name;
	bool _textured;

	unsigned int _tex;

	static bool _wireFrame;
	static Vector3 _wireColor;
	void FillInfo(RenderInfo* info);

};

inline void
Mesh::SetWired(bool wired)
{
	_wireFrame = wired;
}

inline void
Mesh::SetWireColor(const Vector3& color)
{
	_wireColor = color;
}

inline void
Mesh::SetTextured(bool textured)
{
	_textured = textured;
}

inline const std::string&
Mesh::GetName() const
{
	return _name;
}
}