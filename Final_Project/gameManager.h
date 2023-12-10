class GameManager
{
private:
	int score;
	float time;
public:

	GameManager();
	~GameManager();

	int getScore();
	float getTime();

	void setScore(int score);
	void setTime(float time);

	void printResult();
};