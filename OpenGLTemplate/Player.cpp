#include "Player.h"

CPlayer::CPlayer() : m_player(NULL), m_speed(0.25)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialise(COpenAssetImportMesh* object) {
	m_player = object; 
}

void CPlayer::Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera) {
	//m_stack = playerStack; 
	//m_shader = shaderProgram; 
	//m_camera = camera; 

	playerStack.Push();
	playerStack.Translate(m_position);
	playerStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
	playerStack.Scale(2.5f);
	shaderProgram->SetUniform("matrices.modelViewMatrix", playerStack.Top());
	shaderProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(playerStack.Top()));
	m_player->Render();
	playerStack.Pop();
}

void CPlayer::Set(glm::vec3& position, glm::vec3& viewpoint, glm::vec3& upVector) {
	m_position = position + glm::vec3(0.f, 0.f, 6.f);
	m_view = viewpoint;
	m_upVector = upVector; 
}

void CPlayer::Update(double dt)
{
	TranslateByKeyboard(dt);
	m_position.x = glm::clamp(m_position.x, -5.0f, 5.0f);
}

// Update the camera to respond to key presses for translation
void CPlayer::TranslateByKeyboard(double dt)
{
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {
		Advance(1.0 * dt);
	}

	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {
		Advance(-1.0 * dt);
	}

	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		Strafe(-1.0 * dt);
	}

	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		Strafe(1.0 * dt);
	}
}

void CPlayer::Strafe(double direction)
{
	float speed = (float)(m_speed * direction);

	m_position.x = m_position.x + m_strafeVector.x * speed;
	m_position.z = m_position.z + m_strafeVector.z * speed;

	m_view.x = m_view.x + m_strafeVector.x * speed;
	m_view.z = m_view.z + m_strafeVector.z * speed;
}

void CPlayer::Advance(double direction)
{
	float speed = (float)(m_speed * direction);

	glm::vec3 view = glm::normalize(m_view - m_position);
	m_position = m_position + view * speed;
	m_view = m_view + view * speed;

}