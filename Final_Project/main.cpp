#include "point.h"
#include <stdio.h>
#include <iostream>
#include "player.h"
#include "bullet.h"
#include "bullet_line.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "enemy.h"
#include "ground.h"

GLfloat width, height;
GLvoid Reshape(int w, int h);
GLvoid draw();
Shader shader;
Player player;
Ground ground;
Bullet_line bullet_line;
Bullet bullet;
Camera main_camera(glm::vec3(0,2,3),glm::vec3(0,0,0),glm::vec3(0,1,0));		// ī�޶� ��ġ �ٲٷ��� �̰͸� �ٲٸ� ��
Camera minimap_camera(glm::vec3(0, 4, 0), glm::vec3(0, 0, 0),glm::vec3(0,0,1));		//up���� ���� ī�޶�� �ٸ�
Light light;
std::vector<Enemy> enemies;
void update(int value);
GLvoid spawn_enemy(int value);

GLfloat player_rotate = 0.0f;
GLfloat bullet_angle = 0.0f;
int player_move = 0;
bool bang = false;

void update();
void Keyboard(unsigned char key, int x, int y);
void bullet_timer(int value);

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
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
	player.initialize();				//vao,vbo ����
	player.get_shader(shader);

	ground.initialize();
	ground.get_shader(shader);
	bullet_line.initialize();
	bullet_line.get_shader(shader);
	bullet.initialize();
	bullet.get_shader(shader);

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
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid spawn_enemy(int value) {

	enemies.push_back(Enemy(shader));

	glutPostRedisplay();
	glutTimerFunc(5000, spawn_enemy, 1);
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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
	//���� ȭ�� �׸���
	player.draw();		

  for(auto &enemy: enemies) {
	  enemy.draw();
  }
	

	bullet.draw();
	bullet_line.draw();
	ground.draw();

	glViewport(width-200, height-200, 200, 200);
	pTransform = glm::ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	//pTransform = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &pTransform[0][0]);
	minimap_camera.use();
	//�̴ϸ� �׸���
  player.draw();		

	for (auto& enemy : enemies) {
		enemy.draw();
	}

	ground.draw();

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

void update(int value)
{

	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (currentTime - lastTime) / 1000.0f; // �� ������ ��ȯ
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

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case'q':
		player_rotate = 5.0f;
		bullet_angle += player_rotate;

		player.transform = glm::rotate(player.transform, glm::radians(player_rotate), glm::vec3(0.0, 1.0, 0.0));

		bullet_line.transform = glm::rotate(bullet_line.transform, glm::radians(player_rotate), glm::vec3(0.0, 1.0, 0.0));
		
		main_camera.camera_trasform = glm::lookAt(main_camera.eye, main_camera.at, main_camera.up) * glm::rotate(main_camera.rotate_save, glm::radians(-player_rotate), glm::vec3(0.0, 1.0, 0.0));
		main_camera.rotate_save = glm::rotate(main_camera.rotate_save, glm::radians(-player_rotate), glm::vec3(0.0, 1.0, 0.0));
		break;

	case 'e':
		player_rotate = -5.0f;
		bullet_angle -= player_rotate;

		player.transform = glm::rotate(player.transform, glm::radians(player_rotate), glm::vec3(0.0, 1.0, 0.0));

		bullet_line.transform = glm::rotate(bullet_line.transform, glm::radians(player_rotate), glm::vec3(0.0, 1.0, 0.0));
		
		main_camera.camera_trasform = glm::lookAt(main_camera.eye, main_camera.at, main_camera.up) * glm::rotate(main_camera.rotate_save, glm::radians(-player_rotate), glm::vec3(0.0, 1.0, 0.0));
		main_camera.rotate_save = glm::rotate(main_camera.rotate_save, glm::radians(-player_rotate), glm::vec3(0.0, 1.0, 0.0));
		break;

	case 32:
		glutTimerFunc(100, bullet_timer, 1);
		bang = true;
		break;
	}

	glutPostRedisplay();
}

void bullet_timer(int value)
{
	GLfloat x = 0.0f;
	GLfloat y = 0.0f;

	if (bang)
	{
		bullet.transform = glm::translate(bullet.transform, glm::vec3(cos(bullet_angle) * x, sin(bullet_angle) * y, 0.0f));

		x += 0.01;
		y += 0.01;

		glutPostRedisplay();
		glutTimerFunc(100, bullet_timer, 1);
	}
}