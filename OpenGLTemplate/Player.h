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
	glm::vec3 GetUpVector() const{return m_upVector;}
	glm::vec3 GetStrafeVector() const {return m_strafeVector;}
	glm::vec3 GetView() const { return m_view; }
	glm::vec3 GetStart() const { return m_start; }
	glm::vec3 GetDest() const { return m_dest; }

	void Initialise(COpenAssetImportMesh* object);
	void Set(glm::vec3& position, glm::vec3& viewpoint, glm::vec3& upVector, glm::vec3& next, glm::vec3& upDesired);
	void Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera);
	void Update(double dt);
	void TranslateByKeyboard(double dt);
	void Advance(double direction);
	void Strafe(double direction);
	glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

private:
	COpenAssetImportMesh* m_player; 
	glm::mat4 m_playerMatrix{ 1.f };
	glm::vec3 m_position{ 0.f };
	glm::vec3 m_view{0.f, 0.f, -1.f};
	glm::vec3 m_upVector{ 0.f, 1.f, 0.f };
	glm::vec3 m_upDesired{ 0.f, 1.f, 0.f };
	glm::vec3 m_strafeVector{1.f, 0.f, 0.f};
	glm::vec3 m_start, m_dest;
	glm::vec3 rotationAxis{ 0.0f, 1.0f, 0.0f };
	float cosTheta{ 0.f };
	float m_speed;
};
