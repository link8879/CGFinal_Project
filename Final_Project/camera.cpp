#include "camera.h"

Camera::Camera(glm::vec3 eye, glm::vec3 at,glm::vec3 up): eye(eye),at(at),up(up), camera_trasform(1.0f)
{
	
}

Camera::~Camera()
{
	
}

void Camera::use()
{
	//glm::vec3 rotatedDirection = glm::normalize(glm::vec4(at - eye, 1.0f));
	//at = eye + rotatedDirection;

	camera_trasform = glm::lookAt(eye, at, up);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, &camera_trasform[0][0]);
}


void Camera::get_shader(Shader& shaders)
{
	shader = shaders;
}

void Camera::change_camera()
{

	int viewLoc = glGetUniformLocation(shader.ID, "view"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.

	glm::vec3 cameraPos = glm::vec3(eye.x, eye.y, eye.z); //--- 카메라 위치

	glm::vec3 cameraDirection = glm::vec3(at.x, at.y, at.z); //--- 카메라 바라보는 방향
	glm::vec3 rotatedDirection = glm::normalize(glm::vec4(cameraDirection - cameraPos, 1.0f));
	cameraDirection = cameraPos + rotatedDirection;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	
	camera_trasform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera_trasform[0][0]);
}
