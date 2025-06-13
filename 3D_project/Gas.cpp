#include "Gas.h"
#include "Game.h"
Gas::Gas(Game* _game, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	GameObject(position, rotation, scale),
	m_game(_game),
	m_isActive(true)
{
	// get boxCollider size from model
	m_size = m_game->gasModel.getBoxSize();
	m_size = m_size * scale;
}
void Gas::update(std::vector<Barrier*> obstacles)
{
	glm::mat4 worldPosition = glm::mat4(1.0f);
	// move the gas object left along the x axis
	model = glm::translate(worldPosition, glm::vec3(-0.2, 0, 0)) * model;
	// defined Z positions to randomly reset gas object to
	float Zpos[] = { -6.7,0,6.7 };
	int z = rand() % 3;
	float currentX = model[3][0];

	bool conflict = false;
	// check if there are any nearby barriers that would conflict with spawning the gas
	if (getPosition().x < -25 || !m_isActive)
	{
		for (auto b : obstacles)
		{
			if (abs(b->getPosition().x - 20) < 10.0f) //conflict, too close to a barrier
			{
				conflict = true;
				break;
			}
		}
		if (!conflict) //if there’s no conflict, reset the gas position and activate it
		{
			m_isActive = true;
			setPosition(glm::vec3(20.0f, 0.0f, Zpos[z]));
		}
	}
	// rotate the gas object slowly around the y axis to create a spinning effect
	model = glm::rotate(model,glm::radians(1.1f), glm::vec3(0, 1, 0));


}

void Gas::draw(ShaderProgram& shader, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos)
{
	shader.draw(&m_game->gasModel, &m_game->gasTexture, model, view, projection, cameraPos);
}
