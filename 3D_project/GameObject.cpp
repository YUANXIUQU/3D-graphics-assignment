#include "GameObject.h"

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
    m_position(position),
    m_rotation(rotation),
    m_scale(scale)
{
    // apply transformations: translate, rotate then scale
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	model = glm::scale(model, scale);
}

void GameObject::update()
{}

// AABB collision
bool GameObject::isColliding(const glm::vec3& _a, const glm::vec3& _as, const glm::vec3& _b, const glm::vec3& _bs)
{
    glm::vec3 ahs = _as / 2.0f;
    glm::vec3 bhs = _bs / 2.0f;

    if (_a.x > _b.x)
    {
        if (_b.x + bhs.x < _a.x - ahs.x)
        {
            return false;
        }
    }
    else
    {
        if (_a.x + ahs.x < _b.x - bhs.x)
        {
            return false;
        }
    }
    if (_a.z > _b.z)
    {
        if (_b.z + bhs.z < _a.z - ahs.z)
        {
            return false;
        }
    }
    else
    {
        if (_a.z + ahs.z < _b.z - bhs.z)
        {
            return false;
        }
    }
    if (_a.y > _b.y)
    {
        if (_b.y + bhs.y < _a.y - ahs.y)
        {
            return false;
        }
    }
    else
    {
        if (_a.y + ahs.y < _b.y - bhs.y)
        {
            return false;
        }
    }
    return true;
}

glm::vec3 GameObject::aabb_response(glm::vec3& _a, const glm::vec3& _as, const glm::vec3& _b, const glm::vec3& _bs)
{
    float amount = 0.1f;
    float step = 0.1f;

    while (true)
    {
        if (!isColliding(_a, _as, _b, _bs))

            break;
        _a.x += amount;

        if (!isColliding(_a, _as, _b, _bs))

            break;
        _a.x -= amount;
        _a.x -= amount;

        if (!isColliding(_a, _as, _b, _bs))

            break;
        _a.x += amount;
        _a.z += amount;

        if (!isColliding(_a, _as, _b, _bs))

            break;
        _a.z -= amount;
        _a.z -= amount;

        if (!isColliding(_a, _as, _b, _bs))

            break;
        _a.z += amount;
        _a.y += amount;

        if (!isColliding(_a, _as, _b, _bs))

            break;
        _a.y -= amount;
        _a.y -= amount;

        if (!isColliding(_a, _as, _b, _bs))
            break;
        _a.y += amount;

        amount += step;
    }
    return _a;
}