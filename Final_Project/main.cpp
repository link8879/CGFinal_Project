#include "point.h"
#include "shader.h"
#include <stdio.h>
#include <iostream>

GLfloat width, height;
GLvoid Reshape(int w, int h);
GLvoid draw();
Shader shader;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{

	//const char* objFilePath = "cube.obj";
	//FILE* file = fopen(objFilePath, "r"); // "r"�� �б� ��带 ��Ÿ���ϴ�.
	//ReadObj(file, cube[0]);
	//ReadObj(file, cube[1]);
	//ReadObj(file, cube[2]);
	//ReadObj(file, cube[3]);
	//ReadObj(file, cube[4]);
	//ReadObj(file, cube[5]);
	//ReadObj(file, cube[6]);
	//ReadObj(file, light);

	//fclose(file);

	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(200, 30);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	shader.make_shaderProgram();
	//InitBuffer();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glutDisplayFunc(draw);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}

GLvoid draw()
{
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shader.ID);
}
