#include "point.h"
#include "shader.h"
#include "aabb.h"
#include <vector>

class Boom
{
public:
	AABB calculateAABB(/*GLfloat x, GLfloat y, GLfloat z*/) const;

	glm::vec3 boomPos = glm::vec3(0, 0, 0);

	~Boom();
	Boom(float x, float z);
};