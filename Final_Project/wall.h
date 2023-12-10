#pragma once
#include "point.h"
#include "shader.h"

#include <vector>
#pragma once
class Wall
{
public:
	GLuint VAO = 0, VBO = 0;
	std::vector<Point> vertex;
	glm::mat4 transform;
	glm::vec3 color;
	Shader shader;

	~Wall();
	Wall(float move_x, float move_z,float angle);

	void ReadObj(FILE* path, std::vector<Point>& vertexes);
	void draw();
	void initialize();
	void get_shader(Shader& shader);
};
