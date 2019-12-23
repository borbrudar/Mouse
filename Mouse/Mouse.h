#pragma once
#include <SFML/Graphics.hpp>
#include <random>
class Mouse {
public:
	void move(int board[8][8], sf::Vector2i &boardPos, int reward[8][8]);

	int bestMove(sf::Vector2i boardPos);

	float qtable[4][64] = { 0 };
	float discount = 0.9f;
	float lr = 0.1f;
	float epsilon = 1.f;
};