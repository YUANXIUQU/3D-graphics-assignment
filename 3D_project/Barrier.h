#pragma once
#include "GameObject.h"

class Game;

class Barrier :public GameObject
{
private:
	glm::vec3 m_size;
	glm::vec3 m_scale;
	bool m_isActive;
	Game* m_game;
public:
	Barrier(Game* _game,glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void initSize();
	void update();
	void draw(ShaderProgram& shader, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos);
	void setActive(bool _state)
	{
		m_isActive = _state;
	}
	bool getState()
	{
		return m_isActive;
	}

	glm::vec3 getSize()
	{
		return m_size;
	}
};

