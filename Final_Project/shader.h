#pragma once
#include "point.h"

class Shader
{
public:
	GLuint ID; //--- ���̴� ���α׷� �̸�
	GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
	GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

	Shader();

	~Shader();

	char* filetobuf(const char* file);

	void make_vertexShaders();

	void make_fragmentShaders();

	void make_shaderProgram();
	

};
