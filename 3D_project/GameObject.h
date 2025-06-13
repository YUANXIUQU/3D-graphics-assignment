#pragma once
#include "ShaderProgram.h"
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
class GameObject {
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
public:
	glm::mat4 model;
	GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	virtual void update();
	bool isColliding(const glm::vec3& _a, const glm::vec3& _as, const glm::vec3& _b, const glm::vec3& _bs);
	glm::vec3 aabb_response(glm::vec3& _a, const glm::vec3& _as, const glm::vec3& _b, const glm::vec3& _bs);

	glm::vec3 getPosition()
	{
		m_position.x = model[3][0];
		m_position.y = model[3][1];
		m_position.z = model[3][2];
		return m_position;
	}

	void setPosition(glm::vec3 _position)
	{
		m_position = _position;
		model[3][0] = m_position.x;
		model[3][1] = m_position.y;
		model[3][2] = m_position.z;
	}
};


