#include "Bomb.h"

CBomb::CBomb() : m_is_active(true), m_timer(0.f), m_is_triggered(false), explodeFactor(.5f)
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
	m_is_triggered = false;
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
		shaderProgram->SetUniform("bExplodeObject", m_is_triggered);
		shaderProgram->SetUniform("explodeFactor", explodeFactor * explodeFactor);
		m_bomb->Render();
		matrixStack.Pop();
	}
}

void CBomb::Update(float dt, const glm::vec3& player_pos, int& health)
{
	float distance = glm::distance(player_pos, m_position);

	if ((m_is_active == true) && distance < 4.0f)
	{
		m_is_triggered = true;
		//explodeFactor += dt * 0.02f;

		health -= 10;
	}
	else if (m_is_triggered == true)
	{
		if (distance < 25.f)
			explodeFactor += dt * 0.02f;
		else
		{
			m_is_active = false;
			CBomb::~CBomb();
		}
	}
}
