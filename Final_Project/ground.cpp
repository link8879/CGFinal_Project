#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "point.h"
#include "ground.h"
Ground::Ground()
{
	const char* objFilePath = "cube.obj";
	FILE* file = fopen(objFilePath, "r"); // "r"�� �б� ��带 ��Ÿ���ϴ�.
	ReadObj(file, vertex);
	fclose(file);

	color.x = 1.0;
	color.y = 1.0;
	color.z = 1.0;
	transform = glm::mat4(1.0f);
}

Ground::~Ground()
{

}

void Ground::get_shader(Shader& shaders)
{
	shader = shaders;
}


void Ground::ReadObj(FILE* path, std::vector<Point>& vertexes)
{
	char bind[128];
	memset(bind, 0, sizeof(bind));
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> faces;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> uvData;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> normalData;

	fseek(path, 0, SEEK_SET);

	while (!feof(path)) {
		fscanf(path, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			glm::vec3 vertex;
			fscanf(path, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			unsigned int temp_face[3], temp_uv[3], temp_normal[3];
			fscanf(path, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&temp_face[0], &temp_uv[0], &temp_normal[0],
				&temp_face[1], &temp_uv[1], &temp_normal[1],
				&temp_face[2], &temp_uv[2], &temp_normal[2]);

			faces.emplace_back(temp_face[0] - 1, temp_face[1] - 1, temp_face[2] - 1);
			uvData.emplace_back(temp_uv[0] - 1, temp_uv[1] - 1, temp_uv[2] - 1);
			normalData.emplace_back(temp_normal[0] - 1, temp_normal[1] - 1, temp_normal[2] - 1);
		}
		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
			glm::vec2 uv;
			fscanf(path, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		else if (bind[0] == 'v' && bind[1] == 'n') {
			glm::vec3 normal;
			fscanf(path, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
	}

	// �ʿ��� ��� �о�� ���� ���� ���� � ����
	for (int i = 0; i < faces.size(); ++i) {
		vertexes.push_back(Point(vertices[faces[i].x].x, vertices[faces[i].x].y, vertices[faces[i].x].z, normals[normalData[i].x].x, normals[normalData[i].x].y, normals[normalData[i].x].z));
		vertexes.push_back(Point(vertices[faces[i].y].x, vertices[faces[i].y].y, vertices[faces[i].y].z, normals[normalData[i].y].x, normals[normalData[i].y].y, normals[normalData[i].y].z));
		vertexes.push_back(Point(vertices[faces[i].z].x, vertices[faces[i].z].y, vertices[faces[i].z].z, normals[normalData[i].z].x, normals[normalData[i].z].y, normals[normalData[i].z].z));
	}
}
void Ground::initialize()
{
	glGenVertexArrays(1, &VAO); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(VAO); //--- VAO�� ���ε��ϱ�

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Point), vertex.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Enable �ʼ�! ����ϰڴ� �ǹ�
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)offsetof(struct Point, x));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)offsetof(struct Point, nx));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Ground::draw()
{
	glBindVertexArray(VAO);

	int objColorLocation = glGetUniformLocation(shader.ID, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform3f(objColorLocation, color.x, color.y, color.z);

	int modelLoc = glGetUniformLocation(shader.ID, "model"); //--- ���ؽ� ���̴����� ���� ��ȯ ��� �������� �޾ƿ´�.

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -1.0, 0.0)) * glm::scale(glm::mat4(1.0f), glm::vec3(10.0, 1.0, 10.0));
	// transform = glm::scale(glm::mat4(1.0f), glm::vec3(10.0, 0.0, 10.0));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transform[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, vertex.size());
	glBindVertexArray(0);
}