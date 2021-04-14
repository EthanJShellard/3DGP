#include "Light.h"

Light::Light()
{
	position = glm::vec3(0);
	colour = glm::vec3(1);
}

Light::Light(glm::vec3 _position, glm::vec3 _colour)
{
	position = _position;
	colour = _colour;
}
