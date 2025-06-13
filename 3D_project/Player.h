#pragma once
#include <iostream>
#include "GameObject.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Game;

class Player:public GameObject
{
private:
	glm::vec3 m_size;
	glm::vec3 m_scale;
	Game* m_game;
public:
	Player(Game* _game,glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void initSize();
	void update();
	void draw(ShaderProgram& shader,glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos);
	glm::vec3 getSize()
	{
		return m_size;
	}
};




