#include "gameManager.h"

#include <iostream>

#include "time.h"

GameManager::GameManager()
{
	time_ = time(0);
	setScore(0);
	bullet_counter = 0;
	grenade_counter = 5;
}
GameManager::~GameManager()
{
	
}

void GameManager::setScore(int scores)
{
	score = scores;
}

void GameManager::setTime(time_t times)
{
	time_ = times;
}

int GameManager::getScore()
{
	return score;
}

time_t GameManager::getTime()
{
	return time_;
}


void GameManager::printResult()
{
	std::cout << "Game Over" << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "Score: " <<  getScore() <<std::endl;
	std::cout << "Time: " << getTime() << std::endl;
}

