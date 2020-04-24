#include "Pickup.h"

CPickup::CPickup() : m_is_active(true), m_timer(0.f), m_is_triggered(false), explodeFactor(0.0f)
{}

CPickup::CPickup(CSphere*& object, glm::vec3& pickup_pos) : m_is_active(true)
{
	m_pickup = object;
	m_position = pickup_pos;
}

CPickup::~CPickup()
{}

void CPickup::Initialise(CSphere* &object)
{
	m_pickup = object;
	m_is_active = true;
	m_is_triggered = false;
}

void CPickup::SetPosition(glm::vec3& pickup_pos)
{
	m_position = pickup_pos;
}

void CPickup::Render(glutil::MatrixStack matrixStack, CShaderProgram* shaderProgram, CCamera* camera, float dt)
{

	if (m_is_active == true) {
		matrixStack.Push();
		matrixStack.Translate(m_position);
		matrixStack.Scale(2.f);
		shaderProgram->SetUniform("matrices.modelViewMatrix", matrixStack.Top());
		shaderProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(matrixStack.Top()));
		shaderProgram->SetUniform("bExplodeObject", m_is_triggered);
		shaderProgram->SetUniform("explodeFactor", explodeFactor*explodeFactor);
		m_pickup->Render();
		matrixStack.Pop();
	}
}

void CPickup::Update(float dt, const glm::vec3 &player_pos, int &score)
{
	float distance = glm::distance(player_pos, m_position);
		
	if ((m_is_active == true) && distance < 4.0f)
	{
		m_is_triggered = true;
		explodeFactor += dt * 0.02f;

		score += 10;
	}
	else if (m_is_triggered == true && distance > 25.f)
	{
		m_is_active = false;
		CPickup::~CPickup();
	}
}
