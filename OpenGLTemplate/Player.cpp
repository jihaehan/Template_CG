#include "Player.h"

CPlayer::CPlayer()
	:m_player(NULL),
	m_position(0.0f, 5.0f, 0.0f),
	m_rotationAxis(0.0f, 0.1f, 0.f),
	m_rotationAmount(1.0f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialise(COpenAssetImportMesh* object) {
	m_player = object; 
}

void CPlayer::Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera) 
{
	glm::quat RotationQuat = LookAt(m_view, m_upVector);
	float angle = acos(RotationQuat.w) * 2; 
	glm::vec3 axis = {RotationQuat.x/(sin(angle/2)), RotationQuat.y/(sin(angle/2)), RotationQuat.z/(sin(angle/2))};

	playerStack.Push();
	playerStack.Translate(m_position);
	playerStack.RotateRadians(axis, angle);
	playerStack.Scale({ 4.f });
	shaderProgram->SetUniform("matrices.modelViewMatrix", playerStack.Top());
	shaderProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(playerStack.Top()));
	m_player->Render();
	playerStack.Pop();
	
}

void CPlayer::Set(glm::vec3& position,  glm::vec3& viewpoint, glm::vec3& upVector, glm::vec3& RVector, glm::vec3& LVector) {
	
	m_view = viewpoint; 
	m_upVector = upVector; 
	m_position = position + m_speedr * glm::vec3(RVector.x, 0, RVector.y) + m_speedl * glm::vec3(LVector.x, 0, LVector.y) ;
}	

void CPlayer::Update(double dt)
{
	TranslateByKeyboard(dt);
	//m_position.x = glm::clamp(m_position.x, -5.0f, 5.0f);
}

// Update the camera to respond to key presses for translation
void CPlayer::TranslateByKeyboard(double dt)
{
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		m_speedl += 0.01f * dt;
	}

	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		m_speedr += 0.01f * dt;
	}
}

void CPlayer::Advance(double direction)
{ /*
	float speed = (float)(m_speed * direction);

	glm::vec3 view = glm::normalize(m_view - m_position);
	m_position = m_position + view * speed;
	m_view = m_view + view * speed;
	*/
}

// Returns a quaternion such that q*start = dest
glm::quat CPlayer::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	// Implementation from Stan Melax's Game Programming Gems 1 article
	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

// Returns a quaternion that will make your object looking towards 'direction'.
// Similar to RotationBetweenVectors, but also controls the vertical orientation.
// Beware, the first parameter is a direction, not the target point !
glm::quat CPlayer::LookAt(glm::vec3 direction, glm::vec3 desiredUp) {

	if (length2(direction) < 0.0001f)
		return glm::quat();

	// Recompute desiredUp so that it's perpendicular to the direction
	// You can skip that part if you really want to force desiredUp
	m_strafeVector = cross(direction, desiredUp);
	desiredUp = cross(m_strafeVector, direction);

	// Find the rotation between the front of the object (that we assume towards +Z,
	// but this depends on your model) and the desired direction
	glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), direction);	  //direction at rest
	// Because of the 1rst rotation, the up is probably completely screwed up. 
	// Find the rotation between the "up" of the rotated object, and the desired up
	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);								  
	glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

	// Apply them
	return rot2 * rot1; // remember, in reverse order.
}

// Like SLERP, but forbids rotation greater than maxAngle (in radians)
// In conjunction to LookAt, can make your characters 
glm::quat CPlayer::RotateTowards(glm::quat q1, glm::quat q2, float maxAngle) {

	if (maxAngle < 0.001f) {
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;
	}

	float cosTheta = dot(q1, q2);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if (cosTheta > 0.9999f) {
		return q2;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0) {
		q1 = q1 * -1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2° difference, and we are allowed 5°,
	// then we arrived.
	if (angle < maxAngle) {
		return q2;
	}

	// This is just like slerp(), but with a custom t
	float t = maxAngle / angle;
	angle = maxAngle;

	glm::quat res = (sin((1.0f - t) * angle) * q1 + sin(t * angle) * q2) / sin(angle);
	res = normalize(res);
	return res;
}