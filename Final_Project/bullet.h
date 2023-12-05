#include "point.h"
#include "shader.h"
#include "player.h"
#include "aabb.h"
#include <vector>

class Bullet
{
public:
	AABB calculateAABB() const;

	GLuint VAO = 0, VBO = 0;
	std::vector<Point> vertex;
	glm::mat4 transform = glm::mat4(1.0f);
	glm::vec3 color;
	Shader shader;
	glm::mat4 init_transform = glm::mat4(1.0f);
	glm::vec3 bulletPos = glm::vec3(0,0,0);
	float bulletSpeed = 5;

	~Bullet();
	Bullet(Shader& shader,Player player);

	void ReadObj(FILE* path, std::vector<Point>& vertexes);
	void draw();

	void update(float deltaTime, Player player);
};