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
#include "soundManager.h"
#include <algorithm>
#include <vector>
#include "gameManager.h"
#include "boom.h"

GameManager game_manager;
GLfloat width, height;
GLvoid Reshape(int w, int h);
GLvoid draw();
Shader shader;
Player player;
Ground ground;
std::vector<Bullet> bullets;
Camera main_camera(glm::vec3(0,2,3),glm::vec3(0,0,0),glm::vec3(0,1,0));
Camera minimap_camera(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0),glm::vec3(0,0,1));
Light light;
SoundManager sound;
std::vector<Enemy> enemies;
void update(int value);
GLvoid spawn_enemy(int value);
std::vector<Grenade> grenades;

std::vector<Boom> booms;

GLfloat player_rotate = 0.0f;
GLfloat bullet_angle = 0.0f;
int player_move = 0;

bool remove_grenade = false;

bool change_view = false;
bool game_over = false;
int count = 0;
bool bang = false;
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

	sound.playBgm();
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
	glm::mat4 pTransform = glm::mat4(1.0f);

	glViewport(0, 0, width, height);
	

	if(change_view) {
		minimap_camera.use();
		pTransform = glm::ortho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	}
	else {
		main_camera.use();
		pTransform = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 200.0f);
	}
	
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &pTransform[0][0]);

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

	if(change_view) {
		float aspectRatio = 1.0f;
		main_camera.use();
		pTransform = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 200.0f);
	}
	else {
		minimap_camera.use();
		pTransform = glm::ortho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	}
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &pTransform[0][0]);
	

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
	time_t times = time(0);

	for(auto& enemy: enemies) {
		if(times - game_manager.getTime() > 30) {
			enemy.t += deltaTime * 0.125;
		}
		else if(times - game_manager.getTime() > 120) {
			enemy.t += deltaTime * 0.15;
		}
		else {
			enemy.t += deltaTime * 0.1;
		}

		
		enemy.update(enemy.t);
		if(enemy.t >= 1.0) {
			enemy.t = 1;
			// game_over = true;
		
			std::cout << times << std::endl;
			std::cout << game_manager.getTime() << std::endl;
			times = times - game_manager.getTime();
			game_manager.setTime(times);
			game_manager.printResult();
		}

		if (checkAABBCollision(enemy.calculateAABB(), player.calculateAABB())) {
			game_over = true;
			std::cout << "you died" << std::endl;
		}
	}

	for (auto& bullet : bullets) {
		bool removeBullet = false;

		bullet.update(deltaTime, player);

		for (auto it = enemies.begin(); it != enemies.end();) {
			if (checkAABBCollision(bullet.calculateAABB(), it->calculateAABB())) {
				it->hp -= 1;

				removeBullet = true;

				std::cout << "crash" << std::endl;

				if (it->hp == 0) {
					it = enemies.erase(it);
					continue;
				}

				break;
			}

			++it;
		}

		if (removeBullet) {
			bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](const Bullet& b) {
				return &b == &bullet;
				}), bullets.end());
		}
	}

	// grenade and create boom
	for (auto& grenade : grenades) {
		grenade.grenade_y -= 0.01;

		grenade.update(deltaTime, player, grenade.grenade_y);

		if (grenade.grenade_y <= -3.0 && grenade.is_boom) {
			booms.push_back(Boom(grenade.transform[3][0], grenade.transform[3][2]));

			for (auto& boom : booms) {
				for (auto it = enemies.begin(); it != enemies.end();) {
					if (checkAABBCollision(boom.calculateAABB(), it->calculateAABB())) {
						it->hp -= 1;

						grenade.is_boom = false;
						std::cout << "Boom!" << std::endl;

						if (it->hp == 0) {
							it = enemies.erase(it);
							continue;
						}

						break;
					}

					++it;
				}
				
			}

			booms.clear();
		}
	}

	glutPostRedisplay();
	if(!game_over) {
		glutTimerFunc(1, update, 1);
	}
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
		if(game_manager.bullet_counter < 15) {
			bullets.push_back(Bullet(shader, player));
			std::cout << bullets.size() << std::endl;
			bang = true;
			sound.playShooting();
			game_manager.bullet_counter++;
		}
		break;

	case 'a':
		if (game_manager.grenade_counter > 0) {
			grenades.push_back(Grenade(shader, player));
			std::cout << "grenade: " << game_manager.grenade_counter << std::endl;
			game_manager.grenade_counter--;
		}
		break;
	case 'r':
		if(!change_view) {
			change_view = true;
		}
		else {
			change_view = false;
		}
		
		break;
	}

	glutPostRedisplay();
}
