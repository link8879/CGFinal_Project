#include "point.h"
#include "shader.h"

class Camera
{
public:
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

	glm::mat4 camera_trasform;
	Shader shader;

	Camera(glm::vec3 pos, glm::vec3 look);
	~Camera();

	void initialize();
	void change_camera();
	void get_shader(Shader& shaders);
};