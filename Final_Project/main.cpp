#include "point.h"
#include <stdio.h>
#include <iostream>
#include "player.h"
#include "camera.h"
#include "shader.h"

GLfloat width, height;
GLvoid Reshape(int w, int h);
GLvoid draw();
Shader shader;
Player player;
Camera camera(glm::vec3(0,2,3),glm::vec3(0,0,0));		// 카메라 위치 바꾸려면 이것만 바꾸면 됨

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(200, 30);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	shader.make_shaderProgram();
	player.initialize();				//vao,vbo 설정
	player.get_shader(shader);
	camera.get_shader(shader);		
	camera.initialize();				//카메라 초기 위치 설정
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glutDisplayFunc(draw);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}

GLvoid draw()
{
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기

	player.draw();
	glutSwapBuffers(); //--- 화면에 출력하기
}
