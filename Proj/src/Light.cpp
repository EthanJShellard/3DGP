#include "Light.h"

Light::Light()
{
	transform = std::make_shared<Transform>();
	transform->position = glm::vec3(0);
	colour = glm::vec3(1);
}

Light::Light(glm::vec3 _position, glm::vec3 _colour)
{
	transform = std::make_shared<Transform>();
	transform->position = _position;
	colour = _colour;
}
