#include "point.h"
#include "shader.h"
#include <vector>
#pragma once
class Enemy
{
public:
	GLuint VAO = 0, VBO = 0;
	std::vector<Point> vertex;
	glm::mat4 transform;
	glm::vec3 color;
	Shader shader;
	glm::vec3 init_location;
	float t = 0;

	~Enemy();
	Enemy(Shader& shaders);

	void ReadObj(FILE* path, std::vector<Point>& vertexes);
	void draw();
	void update(float alpha);
};
