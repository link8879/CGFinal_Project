#include "gameManager.h"

#include <iostream>

#include "time.h"

GameManager::GameManager()
{
	setTime(time_t());
	setScore(0);
}
GameManager::~GameManager()
{
	
}

void GameManager::setScore(int scores)
{
	score = scores;
}

void GameManager::setTime(float times)
{
	time = times;
}

int GameManager::getScore()
{
	return score;
}

float GameManager::getTime()
{
	return time;
}


void GameManager::printResult()
{
	std::cout << "Game Over" << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "Score" <<  getScore() <<std::endl;
	std::cout << "Time" << getTime() << std::endl;
}

