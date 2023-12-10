#include "point.h"
#include "shader.h"
#include "player.h"
#include "aabb.h"
#include <vector>

class Grenade
{
public:
	AABB calculateAABB() const;

	GLuint VAO = 0, VBO = 0;
	std::vector<Point> vertex;
	glm::mat4 transform = glm::mat4(1.0f);
	glm::vec3 color;
	Shader shader;
	glm::mat4 init_transform = glm::mat4(1.0f);
	glm::vec3 grenadePos = glm::vec3(0, 0, 0);
	float grenadeSpeed = 5;
	bool is_not_boomed = false;
	float grenade_y = 3.0;

	bool is_boom = false;

	~Grenade();
	Grenade(Shader& shader, Player player);

	void ReadObj(FILE* path, std::vector<Point>& vertexes);
	void draw();

	void update(float deltaTime, Player player, float y);
};