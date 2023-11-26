#pragma once
#include "point.h"
#include "shader.h"

#include <vector>
#pragma once
class Bullet
{
public:
	GLuint VAO = 0, VBO = 0;
	std::vector<Point> vertex;
	glm::mat4 transfom;
	glm::vec3 color;
	Shader shader;

	~Bullet();
	Bullet();

	void ReadObj(FILE* path, std::vector<Point>& vertexes);
	void draw();
	void initialize();
	void get_shader(Shader& shader);
};