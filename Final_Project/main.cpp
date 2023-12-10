#include "point.h"
#include <stdio.h>
#include <iostream>
#include "player.h"
#include "bullet.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "enemy.h"
#include "ground.h"
#include "grenade.h"
#include "aabb.h"
#include "collision.h"
#include <algorithm>
#include <vector>

GLfloat width, height;
GLvoid Reshape(int w, int h);
GLvoid draw();
Shader shader;
Player player;
Ground ground;
std::vector<Bullet> bullets;
std::vector<Grenade> grenades;
Camera main_camera(glm::vec3(0,0,3),glm::vec3(0,0,0),glm::vec3(0,1,0));
Camera minimap_camera(glm::vec3(0, 4, 0), glm::vec3(0, 0, 0),glm::vec3(0,0,1));
Light light;
std::vector<Enemy> enemies;
void update(int value);
GLvoid spawn_enemy(int value);

GLfloat player_rotate = 0.0f;
GLfloat bullet_angle = 0.0f;
int player_move = 0;

void update();
void Keyboard(unsigned char key, int x, int y);

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(200, 30);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	glewInit();
	shader.make_shaderProgram();
	player.initialize();
	player.get_shader(shader);

	ground.initialize();
	ground.get_shader(shader);

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

GLvoid Reshape(int w, int h)
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

	player.draw();		

  for(auto &enemy: enemies) {
	  enemy.draw();
  }

  for (auto& bullet : bullets) {
	  bullet.draw();
  }

  for (auto& grenade : grenades) {
	  grenade.draw();
  }


	ground.draw();

	glViewport(width-200, height-200, 200, 200);
	pTransform = glm::ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	//pTransform = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &pTransform[0][0]);
	minimap_camera.use();

	player.draw();		

	for (auto& enemy : enemies) {
		enemy.draw();
	}

	for (auto& bullet : bullets) {
		bullet.draw();
	}

	for (auto& grenade : grenades) {
		grenade.draw();
	}

	ground.draw();

	glutSwapBuffers();
}

void update(int value)
{

	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	for(auto& enemy: enemies) {
		enemy.t += deltaTime * 0.1;
		enemy.update(enemy.t);
		if(enemy.t >= 1.0) {
			enemy.t = 1;
		}

		if (checkAABBCollision(enemy.calculateAABB(), player.calculateAABB())) {
			std::cout << "플레이어 사망 ㅋ" << std::endl;
		}
	}

	for (auto& bullet : bullets) {
		bool removeBullet = false;

		bullet.update(deltaTime, player);

		for (auto it = enemies.begin(); it != enemies.end(); /* 비어두기 */) {
			if (checkAABBCollision(bullet.calculateAABB(), it->calculateAABB())) {
				// Handle collision (e.g., 적 체력 감소, 총알 제거, 등등)
				it->hp -= 1;

				removeBullet = true;

				// 적의 체력이 0이라면 탈출
				if (it->hp == 0) {
					it = enemies.erase(it);
					continue;
				}

				// 적당 한 번의 총알 충돌 체크 후 탈출
				break;
			}

			// 다음 객체로 이동
			++it;
		}

		// 적 루프 밖에서 총알 제거
		if (removeBullet) {
			bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](const Bullet& b) {
				return &b == &bullet;  // 제거할 총알 식별
				}), bullets.end());
		}
	}



	for (auto& grenade : grenades) {
		grenade.grenade_y -= 0.01;

		grenade.update(deltaTime, player, grenade.grenade_y);

		
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


		glm::mat4 Pt = glm::mat4(1.0f);
		glm::mat4 Pr = glm::mat4(1.0f);
		glm::mat4 Pr2 = glm::mat4(1.0f);

		Pt = glm::translate(Pt, glm::vec3(0.0, -0.5, 0.0));
		Pr = glm::rotate(Pr, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));

		Pr2 = glm::rotate(Pr2, glm::radians(bullet_angle), glm::vec3(0.0, 1.0, 0.0));


		player.transform = Pt * Pr * Pr2;
		
		main_camera.camera_trasform = glm::lookAt(main_camera.eye, main_camera.at, main_camera.up) * glm::rotate(main_camera.rotate_save, glm::radians(-player_rotate), glm::vec3(0.0, 1.0, 0.0));
		main_camera.rotate_save = glm::rotate(main_camera.rotate_save, glm::radians(-player_rotate), glm::vec3(0.0, 1.0, 0.0));
		break;

	case 'e':
	{
		player_rotate = -5.0f;
		bullet_angle += player_rotate;


		glm::mat4 Pt = glm::mat4(1.0f);
		glm::mat4 Pr = glm::mat4(1.0f);
		glm::mat4 Pr2 = glm::mat4(1.0f);

		Pt = glm::translate(Pt, glm::vec3(0.0, -0.5, 0.0));
		Pr = glm::rotate(Pr, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));

		Pr2 = glm::rotate(Pr2, glm::radians(bullet_angle), glm::vec3(0.0, 1.0, 0.0));


		player.transform = Pt * Pr * Pr2;

		main_camera.camera_trasform = glm::lookAt(main_camera.eye, main_camera.at, main_camera.up) * glm::rotate(main_camera.rotate_save, glm::radians(-player_rotate), glm::vec3(0.0, 1.0, 0.0));
		main_camera.rotate_save = glm::rotate(main_camera.rotate_save, glm::radians(-player_rotate), glm::vec3(0.0, 1.0, 0.0));
	}
		
		break;

	case 32:		//space
		bullets.push_back(Bullet(shader,player));
		std::cout << bullets.size() << std::endl;
		break;

	case 'a':
		grenades.push_back(Grenade(shader, player));
		break;
	}

	glutPostRedisplay();
}
