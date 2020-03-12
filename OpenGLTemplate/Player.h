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
	glm::vec3 GetNextPosition() const { return m_positionNext; }
	glm::vec3 GetUpVector() const{return m_upVector;}
	glm::vec3 GetNextVector() const { return m_upDesired; }
	glm::vec3 GetStrafeVector() const {return m_strafeVector;}
	glm::vec3 GetView() const { return m_view; }
	glm::vec3 GetStart() const { return m_start; }
	glm::vec3 GetDest() const { return m_dest; }
	glm::vec3 GetScale() const { return m_scale; }
	glm::vec3 GetRotationAxis() const{ return m_rotationAxis; }
	glm::vec3 GetOffset() const { return m_offset; }
	float GetRotationAmount() const { return m_rotationAmount; }
	COpenAssetImportMesh* GetMesh() const { return m_player; }

	void Initialise(COpenAssetImportMesh* object);
	void Set(glm::vec3& position, glm::vec3& nextPosition, glm::vec3& viewpoint, glm::vec3& upVector, glm::vec3& next, glm::vec3& upDesired);
	void Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera);
	void Update(double dt);
	void TranslateByKeyboard(double dt);
	void Advance(double direction);
	void Strafe(double direction);
	glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
	glm::quat ForceVectorUpright();
	const void Transform(glm::mat4& player_transform);

private:
	COpenAssetImportMesh* m_player; 
	glutil::MatrixStack playerMatrixStack; 

	glm::vec3 m_position{ 0.f };
	glm::vec3 m_positionNext{ 0.f };
	glm::vec3 m_scale{ 1.0f };
	glm::vec3 m_rotationAxis{ 0.0f, 1.0f, 0.0f };
	float	m_rotationAmount{ 0.f };
	glm::vec3 m_offset{ 0.f };

	glm::vec3 m_view{0.f, 0.f, -1.f};
	glm::vec3 m_upVector{ 0.f, 1.f, 0.f };
	glm::vec3 m_upDesired{ 0.f, 1.f, 0.f };
	glm::vec3 m_strafeVector{0.f, 0.f, -1.f};
	glm::vec3 m_start, m_dest;
	float cosTheta{ 0.f };
	float m_speed{ 0.1f };
};
