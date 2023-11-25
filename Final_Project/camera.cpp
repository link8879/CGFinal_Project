#include "camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 look): eye(pos),at(look),up(0,1,0), camera_trasform(1.0f)
{
	
}

Camera::~Camera()
{
	
}

void Camera::initialize()
{
	//glm::vec3 rotatedDirection = glm::normalize(glm::vec4(at - eye, 1.0f));
	//at = eye + rotatedDirection;

	camera_trasform = glm::lookAt(eye, at, up);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, &camera_trasform[0][0]);

	glm::mat4 pTransform = glm::mat4(1.0f);
	pTransform = glm::perspective(glm::radians(60.0f), (float)1000 / (float)1000, 0.1f, 200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &pTransform[0][0]);
}


void Camera::get_shader(Shader& shaders)
{
	shader = shaders;
}

void Camera::change_camera()
{

	int viewLoc = glGetUniformLocation(shader.ID, "view"); //--- ���ؽ� ���̴����� ���� ��ȯ ��� �������� �޾ƿ´�.

	glm::vec3 cameraPos = glm::vec3(eye.x, eye.y, eye.z); //--- ī�޶� ��ġ

	glm::vec3 cameraDirection = glm::vec3(at.x, at.y, at.z); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 rotatedDirection = glm::normalize(glm::vec4(cameraDirection - cameraPos, 1.0f));
	cameraDirection = cameraPos + rotatedDirection;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	
	camera_trasform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera_trasform[0][0]);
}
