#include "Barrier.h"
#include "Game.h"
Barrier::Barrier(Game* _game,glm::vec3 position, glm::vec3 rotation, glm::vec3 _scale) :
    GameObject(position, rotation, _scale),
    m_scale(_scale),
    m_isActive(false),
    m_game(_game)
{
    // Default box size before scaling
    m_size = glm::vec3(1.0f);
}
void Barrier::initSize()
{
    // get boxCollider size from model
    m_size = m_game->barrierModel.getBoxSize();
    m_size = m_size * m_scale;
}
void Barrier::update()
{
    // updates the barrier's world position and checks if it should be deactivated
    glm::mat4 worldPosition = glm::mat4(1.0f);
    // move the gas object left along the X-axis
    model = glm::translate(worldPosition, glm::vec3(-0.2, 0, 0)) * model;

    float currentX = model[3][0];
    if (getPosition().x < -25)
    {
        this->setActive(false); // will be back to pool
    }
}
void Barrier::draw(ShaderProgram& shader, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos)
{
    shader.draw(&m_game->barrierModel, &m_game->barrierTexture, model, view, projection, cameraPos);
}