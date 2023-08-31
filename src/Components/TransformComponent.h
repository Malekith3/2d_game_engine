#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent{
    glm::vec2 m_position;
    glm::vec2 m_scale;
    double m_rotation;

    TransformComponent(glm::vec2 position, glm::vec2 scale, double rotation):
    m_position(position),m_rotation(rotation),m_scale(scale){};
};

#endif