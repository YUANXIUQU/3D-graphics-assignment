#pragma once
#include <iostream>
#include "GameObject.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Game;

class Track :public GameObject
{
private:
	VBO m_roadPositions;
	VBO m_texCoords;
	VBO m_normals;
	VAO m_roadVAO;
	Game* m_game;
public:
	Track(Game* game, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void update();
	void draw(ShaderProgram& shader, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos);
};