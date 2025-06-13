
#include "Player.h"
#include "Game.h"
Player::Player(Game* _game, glm::vec3 position, glm::vec3 rotation, glm::vec3 _scale):
	GameObject(position, rotation, _scale),
	m_game(_game),
	m_scale(_scale)
{
	m_size = glm::vec3(1.0f);
}

void Player::initSize()
{
	//get boxCollider size from model
	m_size = m_game->carModel.getBoxSize();
	m_size = m_size * m_scale;
	
}
void Player::update()
{
	SDL_PumpEvents();
	glm::mat4 worldPosition = glm::mat4(1.0f);
	const Uint8* key = SDL_GetKeyboardState(0);
	// move forward (W) if not beyond the top boundary
	if (getPosition().z >= -10 + 6.67 / 2)
	{
		if (key[SDL_SCANCODE_W])
		{
			worldPosition = glm::translate(worldPosition, glm::vec3(0, 0, -0.2));
			model = worldPosition * model;
		}
	}
	// move backward (S) if not beyond the bottom boundary
	if (getPosition().z <= 10 - 6.67 / 2)
	{
		if (key[SDL_SCANCODE_S])
		{
			worldPosition = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.2));
			model = worldPosition * model;
		}
	}
}

void Player::draw(ShaderProgram& shader,glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos)
{
	shader.draw(&m_game->carModel, &m_game->carTexture, model, view, projection, cameraPos);
}


