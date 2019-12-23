#include "Mouse.h"

void Mouse::move(int board[8][8], sf::Vector2i &boardPos, int reward[8][8])
{
	//random epsilon check
	std::random_device rd;
	//std::mt19937 e2(rd());
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> dist(0, 1);

	int prevPos = boardPos.x + boardPos.y * 8;
	int bestMove;

	if (dist(engine) > epsilon) {
		bestMove = this->bestMove(boardPos);
		if(epsilon > 0.1f) epsilon -= 0.0001f;
	}
	else {
		bool moveChosen = false;
		while (!moveChosen) {

			std::uniform_int_distribution<int> moveD(0, 3);
			int move = moveD(engine);
			if (!(boardPos.x == 0 && move == 0) && !(boardPos.x == 7 && move == 1) &&
				!(boardPos.y == 0 && move == 2) && !(boardPos.y == 7 && move == 3))
			{
				moveChosen = true;
				bestMove = move;
			}
		}
		if(epsilon > 0.1f) epsilon -= 0.0001f;
	}

	//0-left, 1-right,2-up,3-down
	if (bestMove == 0) boardPos.x--;
	if (bestMove == 1) boardPos.x++;
	if (bestMove == 2) boardPos.y--;
	if (bestMove == 3) boardPos.y++;

	//evaluate maxQ
	int curPos = boardPos.x + boardPos.y * 8;
	int r1, r2, r3, r4;
	if (boardPos.x - 1 > 0) r1 = qtable[0][curPos]; else r1 = 0;
	if (boardPos.x + 1 < 8) r2 = qtable[1][curPos]; else r2 = 0;
	if (boardPos.y - 1 > 0) r3 = qtable[2][curPos]; else r3 = 0;
	if (boardPos.y + 1 < 8) r4 = qtable[3][curPos]; else r4 = 0;
	int m1 = std::max(r1, r2);
	int m2 = std::max(r3, r4);
	float maxQ = std::max(m1,m2);
	//update the q-table using the Bellman equation
	qtable[bestMove][prevPos] = qtable[bestMove][prevPos] + lr * (reward[boardPos.x][boardPos.y] + discount * (maxQ)-qtable[bestMove][prevPos]);
	//cap the max qtable value
	if (qtable[bestMove][prevPos] > 1000) qtable[bestMove][prevPos] = 1000;
	if (qtable[bestMove][prevPos] < -1000) qtable[bestMove][prevPos] = -1000;
	
	//if on goal/cat return to the beginning
											
	if (board[boardPos.x][boardPos.y] == 2 || board[boardPos.x][boardPos.y] == 3) {
		boardPos.x = 0; boardPos.y = 0;
	}
	

}

int Mouse::bestMove(sf::Vector2i boardPos)
{
	//debug
	int pos = boardPos.x + boardPos.y * 8;
	bool chosenMove = false;
	//todo find biggest value and return 0 - left, 1-right, 2-up,3-down
	int bestMove = 0;
	int temp = -100;
	for (int i = 0; i < 4; i++) {
		if (boardPos.x == 0 && i == 0) continue;
		if (boardPos.x == 7 && i == 1) continue;
		if (boardPos.y == 0 && i == 2) continue;
		if (boardPos.y == 7 && i == 3) continue;

		int sub;
		if (i == 0) sub = -1; if (i == 1) sub = 1; if (i == 2) sub = -8; if (i == 3) sub = 8;
		if (chosenMove == true) {
			if (qtable[i][pos + sub] > temp) {
				bestMove = i;
				temp = qtable[i][pos + sub];
				chosenMove = true;
			}
		}
		else {
			bestMove = i;
			temp = qtable[i][pos + sub];
			chosenMove = true;
		}
	}
	return bestMove;
}


