#pragma once
#include "Common.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "Shaders.h"
#include "Camera.h"

class CPickup
{
public: 
	CPickup();
	CPickup(CSphere*& object, glm::vec3& pickup_pos);
	~CPickup();         
	
	void Initialise(CSphere* &object);
	void Render(glutil::MatrixStack playerStack, CShaderProgram* shaderProgram, CCamera* camera);
	void Update(float dt, const glm::vec3 &player_pos, int &score);
	void SetPosition(glm::vec3& pickup_pos);
	bool Active() { return m_is_active; };
	glm::vec3 GetPosition() const { return m_position; };

private:
	CSphere* m_pickup{};

	bool m_is_active;	
	float m_colourChange;
	float m_timer;
	float m_is_triggered;
	float explodeFactor;
	glm::vec3 m_position{ -155.f, 140.f, 57.f };

};