#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "point.h"
#include "boom.h"
#include "aabb.h"
#include <iostream>

Boom::Boom(float x, float z)
{
	boomPos = glm::vec3(x, -2.0f, z);
}

Boom::~Boom()
{

}

AABB Boom::calculateAABB(/*GLfloat x, GLfloat y, GLfloat z*/) const {
	glm::vec3 boomMin = boomPos - glm::vec3(10.0f);
	glm::vec3 boomMax = boomPos + glm::vec3(10.0f);
	return AABB(boomMin, boomMax);
}