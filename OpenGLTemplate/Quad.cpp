#include "Common.h"
#include "Quad.h"
#include "include\gl\glew.h"

void CQuad::Create(glm::vec4 colour)
{
	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);

	vboRenderData.Create();
	vboRenderData.Bind();

	glm::vec3 vQuadVertices[4] = 
	{
		// Front face
		glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),

	};

	glm::vec3 vQuadNormal = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec4 vQuadColour = colour;

	for (int i = 0; i < 4; i++)
	{
		vboRenderData.AddData(&vQuadVertices[i], sizeof(glm::vec3));
		vboRenderData.AddData(&vQuadNormal[i/4], sizeof(glm::vec3));
		vboRenderData.AddData(&vQuadColour, sizeof(glm::vec4));
	}

	vboRenderData.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride = 2*sizeof(glm::vec3)+sizeof(glm::vec4);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Normal vectors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)));
	// Colour
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec3)));
}

// Render the cube using six triangle strips
void CQuad::Render()
{
	glBindVertexArray(m_uiVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// Release memory associated with the cube
void CQuad::Release()
{
	glDeleteVertexArrays(1, &m_uiVAO);
	vboRenderData.Release();
}

glm::vec2 CQuad::GetPos()
{
	return m_pos;
}

void CQuad::SetPos(glm::vec2 pos)
{
	m_pos = pos;
}
