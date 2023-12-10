#include "time.h"

class GameManager
{
private:
	int score;
	time_t time_;
public:
	int bullet_counter;
	int grenade_counter;
	GameManager();
	~GameManager();

	int getScore();
	time_t getTime();

	void setScore(int score);
	void setTime(float time);

	void printResult();
};