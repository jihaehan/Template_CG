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

	void Initialise(COpenAssetImportMesh* object);
	void Set(glm::vec3& position, glm::vec3& viewpoint, glm::vec3& upVector);
	void Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera);
	void Update(double dt);
	void TranslateByKeyboard(double dt);

	glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
	glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp);
	glm::quat RotateTowards(glm::quat q1, glm::quat q2, float maxAngle);

private:
	COpenAssetImportMesh* m_player{};
	glutil::MatrixStack playerMatrixStack; 

	glm::vec3 m_position{ 0.f };
	glm::vec3 m_view{0.f, 0.f, -1.f};
	glm::vec3 m_upVector{ 0.f, 1.f, 0.f };
	glm::vec3 m_strafeVector{0.f, 0.f, -1.f};
	float m_speed{0.f};
	float m_fspeed{ 0.f };
	float m_clamp{ 9.f };
	float m_lean{ 0.f };

	glm::quat RotationQuat{};
};
