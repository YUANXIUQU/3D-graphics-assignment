#pragma once
#include "GameObject.h"
#include "Barrier.h"
class Game;

class Gas:public GameObject
{
private:
	glm::vec3 m_size;
	bool m_isActive;
	Game* m_game;
public:
	Gas(Game* _game, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void update(std::vector<Barrier*> obstacles);
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

