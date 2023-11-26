#include "camera.h"
#include <iostream>

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

	camera_trasform = glm::lookAt(eye, at, up);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera_trasform[0][0]);
}
