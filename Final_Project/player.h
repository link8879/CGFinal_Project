#include "point.h"
#include "shader.h"
#include "aabb.h"
#include <vector>
#pragma once
class Player
{
public:
	AABB calculateAABB() const;

	GLuint VAO = 0, VBO = 0;
	std::vector<Point> vertex;
	glm::mat4 transform;
	glm::vec3 color;
	Shader shader;
	glm::vec3 init_location;
	glm::vec3 size;

	~Player();
	Player();

	void ReadObj(FILE* path, std::vector<Point>& vertexes);
	void draw();
	void initialize();
	void get_shader(Shader& shader);
};