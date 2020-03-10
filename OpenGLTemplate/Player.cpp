#include "Player.h"

CPlayer::CPlayer() : m_player(NULL)
{
	m_position = glm::vec3(0.0f, 10.0f, 100.0f);
	m_view = glm::vec3(0.0f, 0.0f, -1.0f);
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	m_speed = 0.025f;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialise(COpenAssetImportMesh* object) {
	m_player = object; 
}

void CPlayer::Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera) {
	
	glm::mat4 m_playerMatrixStack = playerStack.Top(); 
	m_playerMatrixStack = playerStack.Top();
	m_playerMatrixStack = glm::translate(m_position);


	playerStack.Push();
	playerStack.Translate(m_position);
	float angle =  atan2(m_view.x, m_view.z); //horse is looking at me
	playerStack.Rotate(m_upVector, angle);
	shaderProgram->SetUniform("matrices.modelViewMatrix", playerStack.Top());
	shaderProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(playerStack.Top()));
	m_player->Render();
	playerStack.Pop();
}


void CPlayer::Set(glm::vec3& position, glm::vec3& viewpoint, glm::vec3& upVector) {
	m_position = position + m_upVector*.5f + m_view*10.f;
	m_upVector = upVector; 
	m_view = viewpoint;
}

void CPlayer::Update(double dt, CCamera camera)
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
