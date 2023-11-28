#include "point.h"
#include "shader.h"

class Camera
{
public:
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

	glm::mat4 camera_trasform;
	glm::mat4 rotate_save;
	Shader shader;

	Camera(glm::vec3 eye, glm::vec3 at, glm::vec3 up);
	~Camera();

	void use();
	void change_camera();
	void get_shader(Shader& shaders);
};