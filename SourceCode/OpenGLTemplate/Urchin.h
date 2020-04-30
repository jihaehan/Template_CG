#pragma once

#include "Texture.h"
#include "VertexBufferObjectIndexed.h"

class CUrchin
{
public:
	CUrchin();
	~CUrchin();
	void Create(string directory, string front, float spikes);
	void Render();
	void Release();
private:

	struct CVertex {
		glm::vec3 pos; //position
		glm::vec2 tex; //texture coordinates
		glm::vec3 nor; //normals
	};

	UINT m_vao;
	CVertexBufferObjectIndexed m_vbo;
	CTexture m_texture;
	string m_directory;
	string m_filename;
};