#pragma once

#include "include\glm\gtc\matrix_transform.hpp"
#include "vertexBufferObject.h"

// Class for a cube
class CQuad
{
public:
	void Create(glm::vec4 colour);	// Create a cube of the specified colour
	void Render();					// Render the cube
	void Release();					// Release memory associated with cube

	glm::vec2 GetPos();
	void SetPos(glm::vec2 pos);

private:
	UINT m_uiVAO;
	CVertexBufferObject vboRenderData;
	glm::vec2 m_pos; // upper left point
	glm::vec2 m_size; // size
};