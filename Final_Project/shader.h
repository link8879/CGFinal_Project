#pragma once
#include "point.h"

class Shader
{
public:
	GLuint ID; //--- 세이더 프로그램 이름
	GLuint vertexShader; //--- 버텍스 세이더 객체
	GLuint fragmentShader; //--- 프래그먼트 세이더 객체

	Shader();

	~Shader();

	char* filetobuf(const char* file);

	void make_vertexShaders();

	void make_fragmentShaders();

	void make_shaderProgram();
	

};
