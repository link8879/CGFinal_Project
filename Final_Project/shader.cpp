#define _CRT_SECURE_NO_WARNINGS
#include "shader.h"
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
Shader::Shader()
{

}

Shader::~Shader()
{
	
}


char* Shader::filetobuf(const char* file)
{
		FILE* fptr;
		long length;
		char* buf;
		fptr = fopen(file, "rb"); // Open file for reading
		if (!fptr) // Return NULL on failure
			return NULL;
		fseek(fptr, 0, SEEK_END); // Seek to the end of the file
		length = ftell(fptr); // Find out how many bytes into the file we are
		buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
		fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
		fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
		fclose(fptr); // Close the file
		buf[length] = 0; // Null terminator
		return buf; // Return the buffer
}

void Shader::make_vertexShaders()
{
		GLchar* vertexSource;
		//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
		//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
		vertexSource = filetobuf("vertex.glsl");
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
			std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
			return;
		}
}
	//--- �����׸�Ʈ ���̴� ��ü �����
void Shader::make_fragmentShaders()
{
		GLchar* fragmentSource;
		//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
		fragmentSource = filetobuf("light_fragment.glsl"); // �����׼��̴� �о����
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
			std::cerr << "ERROR: frag_shader ������ ����\n" << errorLog << std::endl;
			return;
		}
}

void Shader::make_shaderProgram()
{
		this->make_vertexShaders(); //--- ���ؽ� ���̴� �����
		this->make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
		//-- shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		//--- ���̴� �����ϱ�
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		//--- Shader Program ����ϱ�
		glUseProgram(ID);
}





