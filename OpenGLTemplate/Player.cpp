#include "Player.h"

CPlayer::CPlayer()
	:m_player(NULL),
	m_position(0.0f, 5.0f, 0.0f),
	m_rotationAxis(0.0f, 0.1f, 0.f),
	m_rotationAmount(1.0f),
	m_scale(1.0f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialise(COpenAssetImportMesh* object) {
	m_player = object; 
	
}

const void CPlayer::Transform(glm::mat4& player_transform)
{
	player_transform = glm::translate(player_transform, GetPosition() - GetOffset() * GetScale());
	player_transform = glm::rotate(player_transform, GetRotationAmount(), GetRotationAxis());
	player_transform = glm::scale(player_transform, GetScale());
}

glm::quat CPlayer::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
	m_start = glm::normalize(start);
	m_dest = glm::normalize(dest);

	cosTheta = glm::dot(m_start, m_dest);

	if (cosTheta < -1 + 0.001f) //special case when vectors are in opposite directions
	{
		m_rotationAxis = glm::cross(m_upVector, m_start);		//guess one perpendicular vector
		if (glm::length2(m_rotationAxis) < 0.01f)				//if it doesn't work
			m_rotationAxis = glm::cross(m_strafeVector, m_start);	//guess the other direction

		m_rotationAxis = glm::normalize(m_rotationAxis);
		return glm::angleAxis(glm::radians(180.f), m_rotationAxis);
	}
	m_rotationAxis = glm::cross(m_start, m_dest);

	float mag = sqrt((1 + cosTheta) * 2);
	float invs = 1 / mag;

	return glm::quat(
		mag * 0.5f,
		m_rotationAxis.x * invs,
		m_rotationAxis.y * invs,
		m_rotationAxis.z * invs
	);
}

glm::quat CPlayer::ForceVectorUpright()
{
	glm::quat R = RotationBetweenVectors(GetStart(), GetDest());
	glm::vec3 right = glm::cross(GetDest(), m_upDesired);
	m_upDesired = glm::cross(right, GetDest());
	glm::vec3 newUp = R * glm::vec3(m_upVector);
	glm::quat U = RotationBetweenVectors(newUp, m_upDesired);

	//combined quaternion
	glm::quat target = U * R;
	//glm::vec3 targetRot = glm::normalize(glm::vec3(target.x, target.y, target.z));
	//float targetAngle = 2 * acos(target.w);
	
	return target; 
}
void CPlayer::Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera) 
{
	glm::quat R = RotationBetweenVectors(GetStart(), GetDest());

	playerStack.Push();
	playerStack.Translate(m_position + m_upVector + m_view * 10.f);
	playerStack.RotateQuat(glm::toMat4(R));
	playerStack.Scale(.01f);
	shaderProgram->SetUniform("matrices.modelViewMatrix", playerStack.Top());
	shaderProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(playerStack.Top()));
	m_player->Render();
	playerStack.Pop();
	
}


void CPlayer::Set(glm::vec3& position, glm::vec3& nextPosition,  glm::vec3& viewpoint, glm::vec3& upVector, glm::vec3& next, glm::vec3& upDesired) {
	
	m_view = viewpoint; 
	m_upVector = upVector; 
	m_position = position;
	m_start = viewpoint; 
	m_dest = next; 
	m_upDesired = upDesired;
	m_positionNext = nextPosition;

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

