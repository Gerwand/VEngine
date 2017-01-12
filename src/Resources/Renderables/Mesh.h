#pragma once

#include "Renderable.h"
#include "Resources/Managers/TextureManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>



namespace vengine {

/* Class representing basic mesh */
class Mesh : public Renderable 
{
public:
	/* Should always be used before drawing */
	void Init(const std::string& name);

	/* Set if object should be drawn with texture or with color */
	void SetTextured(bool textured);
	/* Set texture for the object using handle from textureManager */
	void SetTexture(unsigned int tex);
	/* Set texture with given name */
	void SetTexture(const std::string& name);

	/* Get texture binded to mesh */
	unsigned int GetTexture() const;
	const std::string& GetName() const;

	/* Draw mesh */
	virtual void Draw(Renderer* renderer);

	/* Set if the mesh should be drawn wired */
	static void SetWired(bool wired);
	/* Set color of the wireframe */
	static void SetWireColor(const Vector3& color);
protected:
	std::string _name;
	bool _textured;		/* Determines if obejct should be drawn with texture or with color */

	unsigned int _tex;	/* Handle for the texture */

	static bool _wireFrame;	/* If true, obejct will be drawn as lines */
	static Vector3 _wireColor; /* Set color of the wires */

	/* Fill info for mesh object */
	virtual void FillInfo(RenderInfo* info);

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