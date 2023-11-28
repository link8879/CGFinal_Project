#include "point.h"
#include <stdio.h>
#include <iostream>
#include "player.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "enemy.h"

GLfloat width, height;
GLvoid Reshape(int w, int h);
GLvoid draw();
Shader shader;
Player player;
Camera main_camera(glm::vec3(0,2,3),glm::vec3(0,0,0),glm::vec3(0,1,0));		// 카메라 위치 바꾸려면 이것만 바꾸면 됨
Camera minimap_camera(glm::vec3(0, 4, 0), glm::vec3(0, 0, 0),glm::vec3(0,0,1));		//up벡터 메인 카메라와 다름
Light light;
std::vector<Enemy> enemies;
void update(int value);
GLvoid spawn_enemy(int value);

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
	main_camera.get_shader(shader);		
	minimap_camera.get_shader(shader);
	light.setLight(shader);
	enemies.reserve(50);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glutDisplayFunc(draw);
	glutReshapeFunc(Reshape);
	glutTimerFunc(5000, spawn_enemy, 1);
	glutTimerFunc(1, update, 1);
	glutMainLoop();
}

GLvoid spawn_enemy(int value) {

	enemies.push_back(Enemy(shader));

	glutPostRedisplay();
	glutTimerFunc(5000, spawn_enemy, 1);
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	width = w;
	height = h;
	//glViewport(0, 0, w, h);
	/*glViewport(w - 200, h - 200, w, h);
	pTransform = glm::perspective(glm::radians(60.0f), (float)200 / (float)200, 0.1f, 200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &pTransform[0][0]);*/
}

GLvoid draw()
{
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader.ID);

	glViewport(0, 0, width, height);
	glm::mat4 pTransform = glm::mat4(1.0f);
	pTransform = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &pTransform[0][0]);
	main_camera.use();
	//메인 화면 그리기
	player.draw();		

  for(auto &enemy: enemies) {
	  enemy.draw();
  }

	glViewport(width-200, height-200, 200, 200);
	pTransform = glm::ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	//pTransform = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &pTransform[0][0]);
	minimap_camera.use();
	//미니맵 그리기
  player.draw();		

	for (auto& enemy : enemies) {
		enemy.draw();
	}

	glutSwapBuffers(); //--- 화면에 출력하기
}

void update(int value)
{

	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (currentTime - lastTime) / 1000.0f; // 초 단위로 변환
	lastTime = currentTime;

	for(auto& enemy: enemies) {
		enemy.t += deltaTime * 0.1;
		enemy.update(enemy.t);
	if(enemy.t >= 1.0) {
		enemy.t = 1;
	}
	}

	glutPostRedisplay();
	glutTimerFunc(1, update, 1);
}