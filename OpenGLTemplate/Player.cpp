#include "Player.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

//Initialize some mesh as the player
void CPlayer::Initialise(COpenAssetImportMesh* object) {
	m_player = object; 
}

//Render Player
void CPlayer::Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera) 
{
	glm::quat RotationQuat = LookAt(m_view, m_upVector);
	float angle = acos(RotationQuat.w) * 2 ; 
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

//Set position and direction of player
void CPlayer::Set(glm::vec3& position,  glm::vec3& viewpoint, glm::vec3& upVector) {
	
	m_view = viewpoint;
	m_upVector = upVector; 
	m_position = position + m_speed *glm::normalize(m_strafeVector) + m_fspeed * glm::normalize(viewpoint);
}	

// Update player movements
void CPlayer::Update(double dt)
{
	TranslateByKeyboard(dt);
}


// Update the camera to respond to key presses for translation
void CPlayer::TranslateByKeyboard(double dt)
{
	//Advance forward
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {		
		if (m_fspeed < m_clamp)
			m_fspeed += 0.01f * dt;
	}
	//Advance backward
	else if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {
		if (m_fspeed > -m_clamp)
			m_fspeed -= 0.01f * dt;
	}
	//Strafe left
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		if (m_lean < m_clamp / 40.f)	//determines a lean factor depending on direction of travel
			m_lean += 0.001f * dt;	
		if (m_speed > -m_clamp) 
			m_speed -= 0.01f * dt - m_lean/5.f;
	}
	//Strafe right
	else if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		if (m_lean > -m_clamp / 40.f)    //determines a lean factor depending on direction of travel
			m_lean -= 0.001f * dt;
		if (m_speed < m_clamp)
			m_speed += 0.01f * dt + m_lean/5.f;
	}
	else 
	{
		if (m_lean > 0.05)	//adjusts player lean so that moving left/right feels more natural
		{
			m_lean -= 0.001f * dt;
			m_speed -= m_lean*m_lean;
		}
		else if (m_lean < -0.05)
		{
			m_lean += 0.001f * dt;
			m_speed -= m_lean*m_lean;
		}
		else
			m_lean = 0;
	}
}

// Returns a quaternion such that q*start = dest
// Equation adapted from : http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#how-do-i-create-a-quaternion-in-c-
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
// Modified equation from http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#how-do-i-create-a-quaternion-in-c-
glm::quat CPlayer::LookAt(glm::vec3 direction, glm::vec3 desiredUp) {

	if (length2(direction) < 0.0001f)
		return glm::quat();

	// Recompute desiredUp so that it's perpendicular to the direction
	// You can skip that part if you really want to force desiredUp
	m_strafeVector = cross(direction, desiredUp);
	desiredUp = cross(m_strafeVector + m_lean*desiredUp, direction);

	// Find the rotation between the front of the object (that we assume towards +Z,
	// but this depends on your model) and the desired direction
	glm::quat rot1 = RotationBetweenVectors(glm::vec3(-m_lean/2.f, 0.0f, 1.0f ), direction);	  //direction at rest
	// Because of the 1rst rotation, the up is probably completely screwed up. 
	// Find the rotation between the "up" of the rotated object, and the desired up
	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);								  
	glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

	// Apply them
	return rot2 * rot1; // remember, in reverse order.
}

// Like SLERP, but forbids rotation greater than maxAngle (in radians)
// In conjunction to LookAt, can make your characters 
// Sourced from: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#how-do-i-create-a-quaternion-in-c-
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