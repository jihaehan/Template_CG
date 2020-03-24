#include "Bomb.h"

CBomb::CBomb() : m_is_active(true)
{}

CBomb::CBomb(CUrchin*& object, glm::vec3& pickup_pos) : m_is_active(true)
{
	m_bomb = object;
	m_position = pickup_pos;
}

CBomb::~CBomb()
{}

void CBomb::Initialise(CUrchin*& object)
{
	m_bomb = object;
	m_is_active = true;
}

void CBomb::SetPosition(glm::vec3& pickup_pos)
{
	m_position = pickup_pos;
}

void CBomb::Render(glutil::MatrixStack matrixStack, CShaderProgram* shaderProgram, CCamera* camera, float dt)
{
	if (m_is_active == true) {
		matrixStack.Push();
		matrixStack.Translate(m_position);
		matrixStack.RotateRadians(glm::vec3(1,1,0), dt/4.f);
		matrixStack.Scale(0.5f);
		shaderProgram->SetUniform("matrices.modelViewMatrix", matrixStack.Top());
		shaderProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(matrixStack.Top()));
		m_bomb->Render();
		matrixStack.Pop();
	}
}

void CBomb::Update(float dt, const glm::vec3& player_pos, int& health)
{
	if ((m_is_active == true) && glm::distance(player_pos, m_position) < 5.0f)
	{
		m_is_active = false;
		health -= 10;
	}
	else if (m_is_active == false)
		CBomb::~CBomb();
}
