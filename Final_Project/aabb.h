#include "point.h"
#include "shader.h"

#ifndef AABB_H
#define AABB_H

struct AABB {
	glm::vec3 min;
	glm::vec3 max;

	AABB() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {}
	AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}
};

#endif // AABB_H