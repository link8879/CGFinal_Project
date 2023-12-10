#define _CRT_SECURE_NO_WARNINGS
#include "enemy.h"

#include <iostream>

#include "shader.h"
#include "random"

Enemy::Enemy(Shader& shaders) {
	

	shader = shaders;
	std::mt19937 generator(std::random_device{}());

	std::uniform_real_distribution<float> distribution_x(-5, 5.0);
	std::uniform_real_distribution<float> distribution_z(-5, 5.0);
	std::uniform_int_distribution<int> rand_hp(1, 3);
	std::uniform_int_distribution<int> rand_shape(0, 1);

	if(rand_shape(generator) == 0) {
		const char* objFilePath = "enemy.obj";
		FILE* file = fopen(objFilePath, "r"); // "r"은 읽기 모드를 나타냅니다.
		ReadObj(file, vertex);
		fclose(file);
	}
	else {
		const char* objFilePath = "enemy2.obj";
		FILE* file = fopen(objFilePath, "r"); // "r"은 읽기 모드를 나타냅니다.
		ReadObj(file, vertex);
		fclose(file);
	}

	hp = rand_hp(generator);

  if(hp == 1) {
	  color.x = 0.5;
	  color.y = 0.5;
	  color.z = 1.0;

	  size = glm::vec3(0.35, 0.35, 0.35);
  }
  else if(hp == 2) {
	  color.x = 0;
	  color.y = 1;
	  color.z = 0.5;

	  size = glm::vec3(0.75, 0.75, 0.75);
  }
  else if(hp == 3) {
	  color.x = 1;
	  color.y = 0.4;
	  color.z = 0.6;
	  size = glm::vec3(1, 1, 1);
  }

	float random_x = distribution_x(generator);
	float random_z = distribution_z(generator);

	if(random_x < 1 && random_z <1) {
		random_x = 4;
	}

	init_location = glm::vec3(random_x, 0, random_z);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(random_x, 0, random_z)) *glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5));

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

Enemy::~Enemy() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Enemy::ReadObj(FILE* path, std::vector<Point>& vertexes) {
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

void Enemy::draw() {
	glBindVertexArray(VAO);

	int objColorLocation = glGetUniformLocation(shader.ID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, color.x, color.y, color.z);

	int modelLoc = glGetUniformLocation(shader.ID, "model"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transform[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, vertex.size());
	glBindVertexArray(0);
}

void Enemy::update(float alpha) {

	glm::vec3 startPosition = init_location;
	glm::vec3 endPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 currentPosition = glm::mix(startPosition, endPosition, alpha);

	transform = glm::translate(glm::mat4(1.0), currentPosition) * glm::scale(glm::mat4(1.0f), size);
}

AABB Enemy::calculateAABB() const {
	glm::vec3 enemyMin = init_location - size / 2.0f;
	glm::vec3 enemyMax = init_location + size / 2.0f;
	return AABB(enemyMin, enemyMax);
}