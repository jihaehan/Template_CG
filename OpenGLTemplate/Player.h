#pragma once
#include "Common.h"
#include "OpenAssetImportMesh.h"
#include "MatrixStack.h"
#include "Shaders.h"
#include "Camera.h"

class CPlayer
{ 
public:
	CPlayer();
	~CPlayer();

	glm::vec3 GetPosition() const{return m_position;}
	glm::vec3 GetView() const{return m_view;}
	glm::vec3 GetUpVector() const{return m_upVector;}
	glm::vec3 GetStrafeVector() const {return m_strafeVector;}

	void Initialise(COpenAssetImportMesh* object);
	void Set(glm::vec3& position, glm::vec3& viewpoint, glm::vec3& upVector);
	void Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera);
	void Update(double dt);
	void TranslateByKeyboard(double dt);
	void Advance(double direction);
	void Strafe(double direction);

private:
	COpenAssetImportMesh* m_player; 
	//glutil::MatrixStack m_stack; 
	//CShaderProgram* m_shader; 
	//CCamera* m_camera; 

	glm::vec3 m_position;			
	glm::vec3 m_view;				
	glm::vec3 m_upVector;			
	glm::vec3 m_strafeVector;		
	float m_speed;				
};
