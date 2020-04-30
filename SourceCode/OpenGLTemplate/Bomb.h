#pragma once
#include "Common.h"
#include "Urchin.h"
#include "MatrixStack.h"
#include "Shaders.h"
#include "Camera.h"

class CBomb
{
public:
	CBomb();
	CBomb(CUrchin*& object, glm::vec3& pickup_pos);
	~CBomb();

	void Initialise(CUrchin*& object);
	void Render(glutil::MatrixStack matrixstack, CShaderProgram* shaderProgram, CCamera* camera, float mt);
	void Update(float dt, const glm::vec3& player_pos, int& score);
	void SetPosition(glm::vec3& pickup_pos);
	bool Active() { return m_is_active; };
	glm::vec3 GetPosition() const { return m_position; };

private:
	CUrchin* m_bomb{};

	bool m_is_active;
	float m_timer;
	float m_is_triggered;
	float explodeFactor;

	glm::vec3 m_position{ -155.f, 140.f, 57.f };

};