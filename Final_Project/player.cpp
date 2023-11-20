#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "point.h"
#include "player.h"
Player::Player()
{
	const char* objFilePath = "test_sphere.obj";
	FILE* file = fopen(objFilePath, "r"); // "r"은 읽기 모드를 나타냅니다.
	ReadObj(file, vertex);
	fclose(file);
}

Player::~Player()
{
	
}

void Player::get_shader(Shader& shaders)
{
	shader = shaders;
}


void Player::ReadObj(FILE* path, std::vector<Point>& vertexes)
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

	// 필요한 경우 읽어온 값을 전역 변수 등에 저장
	for (int i = 0; i < faces.size(); ++i) {
		vertexes.push_back(Point(vertices[faces[i].x].x, vertices[faces[i].x].y, vertices[faces[i].x].z, normals[normalData[i].x].x, normals[normalData[i].x].y, normals[normalData[i].x].z));
		vertexes.push_back(Point(vertices[faces[i].y].x, vertices[faces[i].y].y, vertices[faces[i].y].z, normals[normalData[i].y].x, normals[normalData[i].y].y, normals[normalData[i].y].z));
		vertexes.push_back(Point(vertices[faces[i].z].x, vertices[faces[i].z].y, vertices[faces[i].z].z, normals[normalData[i].z].x, normals[normalData[i].z].y, normals[normalData[i].z].z));
	}
}
void Player::initialize()
{
	glGenVertexArrays(1, &VAO); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(VAO); //--- VAO를 바인드하기

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Point), vertex.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Enable 필수! 사용하겠단 의미
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)offsetof(struct Point, x));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)offsetof(struct Point, nx));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Player::draw()
{
	glBindVertexArray(VAO);
	int objColorLocation = glGetUniformLocation(shader.ID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 0.5, 1.0, 1.0);

	unsigned int lightPosLocation = glGetUniformLocation(shader.ID, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 2.0, 0.0);

	unsigned int lightColorLocation = glGetUniformLocation(shader.ID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);


	int modelLoc = glGetUniformLocation(shader.ID, "model"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	int viewLoc = glGetUniformLocation(shader.ID, "view"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	int projLoc = glGetUniformLocation(shader.ID, "projection");

	glm::vec3 cameraPos = glm::vec3(0.0f , 1.0f, 3.0f); //--- 카메라 위치

	glm::vec3 cameraDirection = glm::vec3(0.0f , 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 rotatedDirection = glm::normalize(glm::vec4(cameraDirection - cameraPos, 1.0f));
	cameraDirection = cameraPos + rotatedDirection;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

	//--- 모델링 변환, 뷰잉 변환, 투영 변환 행렬을 설정한 후, 버텍스 세이더에 저장한다.
	glm::mat4 mTransform = glm::mat4(1.0f);
	//mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glm::mat4 vTransform = glm::mat4(1.0f);
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);

	glm::mat4 pTransform = glm::mat4(1.0f);
	pTransform = glm::perspective(glm::radians(60.0f), (float)1000 / (float)1000, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);


	glDrawArrays(GL_TRIANGLES, 0, vertex.size());
	glBindVertexArray(0);
}



