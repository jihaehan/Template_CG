#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "Tetrahedron.h"
#include <math.h>

CTetrahedron::CTetrahedron()
{}

CTetrahedron::~CTetrahedron()
{}

void CTetrahedron::Create(string a_sDirectory, string a_sFilename, float girth)
{
	// check if filename passed in -- if so, load texture

	m_texture.Load(a_sDirectory+a_sFilename);

	m_directory = a_sDirectory;
	m_filename = a_sFilename;

	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_vbo.Create();
	m_vbo.Bind();

	// Compute vertex attributes and store in VBO
	
	// Compute positions
	std::vector<glm::vec3> positions; //four positions
	positions.push_back(glm::vec3(0.f, 1.5f, 0.f));
	for (int i = 0; i < 3; i++)
	{
		float theta = (i / 3.0f) * 2 * (float)M_PI;
		positions.push_back(glm::vec3(cos(theta), 0.f, sin(theta)) * girth);
	}

	// Compute texture coordinates (for each face)
	std::vector<glm::vec2> texcoord; 
	texcoord.push_back(glm::vec2(0.f, 0.f));
	texcoord.push_back(glm::vec2(1.0f, 0.f));
	texcoord.push_back(glm::vec2(0.5, 1.f));

	// Compute normals
	std::vector<glm::vec3> normals;
	normals.push_back(glm::cross(positions.at(0) - positions.at(2), positions.at(0) - positions.at(1))); 	//front normal
	normals.push_back(glm::cross(positions.at(0) - positions.at(3), positions.at(0) - positions.at(2)));	//left normal
	normals.push_back(glm::cross(positions.at(0) - positions.at(1), positions.at(0) - positions.at(3)));	//right normal
	normals.push_back(glm::cross(positions.at(1) - positions.at(2), positions.at(1) - positions.at(3)));	//bottom normal

	// Compute vertices for VBO
	std::vector<CVertex> vertices;
	CVertex v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11;
	v0.pos = positions.at(0);		v0.tex = texcoord.at(0);		v0.nor = normals.at(0);
	v1.pos = positions.at(2);		v1.tex = texcoord.at(1);		v1.nor = normals.at(0);
	v2.pos = positions.at(1);		v2.tex = texcoord.at(2);		v2.nor = normals.at(0);
	v3.pos = positions.at(0);		v3.tex = texcoord.at(0);		v3.nor = normals.at(1);
	v4.pos = positions.at(3);		v4.tex = texcoord.at(1);		v4.nor = normals.at(1);
	v5.pos = positions.at(2);		v5.tex = texcoord.at(2);		v5.nor = normals.at(1);
	v6.pos = positions.at(0);		v6.tex = texcoord.at(0);		v6.nor = normals.at(2);
	v7.pos = positions.at(1);		v7.tex = texcoord.at(1);		v7.nor = normals.at(2);
	v8.pos = positions.at(3);		v8.tex = texcoord.at(2);		v8.nor = normals.at(2);
	v9.pos = positions.at(1);		v9.tex = texcoord.at(0);		v9.nor = normals.at(3);
	v10.pos = positions.at(2);		v10.tex = texcoord.at(1);		v10.nor = normals.at(3);
	v11.pos = positions.at(3);		v11.tex = texcoord.at(2);		v11.nor = normals.at(3);

	//Add Vertex Data to GPU
	UINT sizeof_vertex = sizeof(glm::vec3) * 2 + sizeof(glm::vec2);
	m_vbo.AddVertexData(&v0, sizeof_vertex);
	m_vbo.AddVertexData(&v1, sizeof_vertex);
	m_vbo.AddVertexData(&v2, sizeof_vertex);
	m_vbo.AddVertexData(&v3, sizeof_vertex);
	m_vbo.AddVertexData(&v4, sizeof_vertex);
	m_vbo.AddVertexData(&v5, sizeof_vertex);
	m_vbo.AddVertexData(&v6, sizeof_vertex);
	m_vbo.AddVertexData(&v7, sizeof_vertex);
	m_vbo.AddVertexData(&v8, sizeof_vertex);
	m_vbo.AddVertexData(&v9, sizeof_vertex);
	m_vbo.AddVertexData(&v10, sizeof_vertex);
	m_vbo.AddVertexData(&v11, sizeof_vertex);

	std::vector<GLuint> indices;
	indices.push_back(0);			indices.push_back(1);		indices.push_back(2);		//front triangle
	indices.push_back(3);			indices.push_back(4);		indices.push_back(5);		//left triangle
	indices.push_back(6);			indices.push_back(7);		indices.push_back(8);		//right triangle
	indices.push_back(9);			indices.push_back(10);		indices.push_back(11);		//bottom triangle

	for (GLuint i = 0; i < indices.size(); i++)
	{
		m_vbo.AddIndexData(&indices[i], sizeof(unsigned int));
	}
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

// Render the sphere as a set of triangles
void CTetrahedron::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

}

// Release memory on the GPU 
void CTetrahedron::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}

	
	