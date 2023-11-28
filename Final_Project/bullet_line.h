#include "point.h"
#include "shader.h"

#include <vector>

class Bullet_line
{
public:
	GLuint VAO = 0, VBO = 0;
	std::vector<Point> vertex;
	glm::mat4 transform;
	glm::vec3 color;
	Shader shader;

	~Bullet_line();
	Bullet_line();

	void ReadObj(FILE* path, std::vector<Point>& vertexes);
	void draw();
	void initialize();
	void get_shader(Shader& shader);
};