#include "Player.h"


CPlayer::CPlayer() : m_player(NULL)
{
	m_speed = 0.025f;
	m_position = glm::vec3(0.0f, 1.0f, 3.0f);
	m_view = glm::vec3(0.0f, 0.0f, -1.0f);
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialise(COpenAssetImportMesh* object) {
	m_player = object; 
}

glm::quat CPlayer::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
	m_start = glm::normalize(start);
	m_dest = glm::normalize(dest);

	cosTheta = dot(m_start, m_dest);

	if (cosTheta < -1 + 0.001f) 
	{
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), m_start);
		if (glm::length2(rotationAxis) < 0.01f)
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), m_start);

		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.f), rotationAxis);
	}
	rotationAxis = glm::cross(m_start, m_dest);
	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

void CPlayer::Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera) {
	//rotation bw front of object and desired location
	glm::quat R = RotationBetweenVectors(GetStart(), GetDest());
	glm::vec3 rot = glm::normalize(glm::vec3(R.x, R.y, R.z));
	float angle = acos(R.w);

	//force vector upright
	glm::vec3 right = glm::cross(GetDest(), m_upDesired);
	m_upDesired = glm::cross(right, GetDest());
	glm::vec3 newUp = rot * glm::vec3(m_upVector);
	glm::quat U = RotationBetweenVectors(newUp, m_upDesired);
	glm::vec3 rot2 = glm::normalize(glm::vec3(U.x, U.y, U.z));
	float upangle = acos(U.w);

	//combined quaternion
	glm::quat target = U * R; // remember, in reverse order.
	glm::vec3 targetRot = glm::normalize(glm::vec3(target.x, target.y, target.z));
	float targetAngle = acos(target.w);

	// kind of working
	playerStack.Push();
	playerStack.Translate(m_position + m_upVector + m_view*10.f);
	playerStack.RotateRadians(targetRot, targetAngle);
	playerStack.Scale(2.f);
	shaderProgram->SetUniform("matrices.modelViewMatrix", playerStack.Top());
	shaderProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(playerStack.Top()));
	m_player->Render();
	playerStack.Pop();

}

void CPlayer::Set(glm::vec3& position, glm::vec3& viewpoint, glm::vec3& upVector, glm::vec3& next, glm::vec3& upDesired) {
	m_view = viewpoint; 
	m_upVector = upVector; 
	m_position = position;
	m_start = viewpoint; 
	m_dest = next; 
	m_upDesired = upDesired;

}

void CPlayer::Update(double dt)
{
	glm::vec3 vector = glm::cross(m_view - m_position, m_upVector);
	m_strafeVector = glm::normalize(vector);
	//TranslateByKeyboard(dt);
	//m_position.x = glm::clamp(m_position.x, -5.0f, 5.0f);
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
