#include "Pickup.h"

CPickup::CPickup() : m_is_active(true)
{}

CPickup::~CPickup()
{}

void CPickup::Initialise(CSphere* &object)
{
	m_pickup = object;
	m_is_active = true;
}

void CPickup::Render(glutil::MatrixStack matrixStack, CShaderProgram* shaderProgram, CCamera* camera)
{
	if (m_is_active == true) {
		matrixStack.Push();
		matrixStack.Translate(m_position);
		matrixStack.Scale(2.f);
		shaderProgram->SetUniform("matrices.modelViewMatrix", matrixStack.Top());
		shaderProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(matrixStack.Top()));
		m_pickup->Render();
		matrixStack.Pop();
	}
}

void CPickup::Update(float dt, const glm::vec3 &player_pos, float &score)
{
	if ((m_is_active == true) && glm::distance(player_pos, m_position) < 4.0f)
	{
		m_is_active = false;
		score += dt * 10.f;
	}
	else if (m_is_active == false)
		CPickup::~CPickup();
}
