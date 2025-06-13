#include "Track.h"
#include "Game.h"

Track::Track(Game* _game, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	GameObject(position, rotation, scale),
    m_game(_game)
{
    // define the road positions (vertices of a square)
    m_roadPositions.add(glm::vec3(-25.0f, 0.0f, -10.0f));
    m_roadPositions.add(glm::vec3(25.0f, 0.0f, -10.0f));
    m_roadPositions.add(glm::vec3(25.0f, 0.0f, 10.0f));
    m_roadPositions.add(glm::vec3(-25.0f, 0.0f, 10.0));

    // define texture coordinates
    m_texCoords.add(glm::vec2(0.0f, 0.0f));
    m_texCoords.add(glm::vec2(1.0f, 0.0f));
    m_texCoords.add(glm::vec2(1.0f, 1.0f));
    m_texCoords.add(glm::vec2(0.0f, 1.0f));

    // define normals for lighting calculations
    m_normals.add(glm::vec3(0,1,0));
    m_normals.add(glm::vec3(0,1,0));
    m_normals.add(glm::vec3(0,1,0));
    m_normals.add(glm::vec3(0,1,0));

    m_roadVAO.setBuffer(&m_roadPositions);
    m_roadVAO.setBuffer(&m_texCoords);
    m_roadVAO.setBuffer(&m_normals);
}

void Track::update()
{
    // move the road (track) along the X axis (simulate motion)
	model = glm::translate(model, glm::vec3(-0.2, 0, 0));

    float currentX = model[3][0];
    // if the track has moved too far left (past -50), reset its position
    if (currentX < -50) {
        float resetDistance = 50 * 2;
        model = glm::translate(model, glm::vec3(resetDistance, 0, 0));
    }
}

void Track::draw(ShaderProgram& shader, glm::mat4& view, glm::mat4& projection, glm::vec3& cameraPos)
{
    shader.draw(&m_roadVAO, &m_game->roadTexture, model, view, projection, cameraPos);
}
