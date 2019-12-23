#include <SFML/Graphics.hpp>
#include "Mouse.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <math.h>

int main() {
	//window and event creation
	sf::RenderWindow window;
	window.create(sf::VideoMode(640, 640), "Mouse reinforcement learning");
	window.setFramerateLimit(60);

	sf::Event e;
	
	//Text
	sf::Font f;
	f.loadFromFile("res/arial/ARI.ttf");
	sf::Text t;
	t.setFont(f);
	t.setCharacterSize(14); 
	t.setFillColor(sf::Color::Black);
	//lines
	sf::RectangleShape line;
	line.setFillColor(sf::Color::Black); line.setSize(sf::Vector2f(640, 2));
	line.setRotation(0.f);

	//mouse, cats, and cheese
	sf::Texture t1, t2, t3;
	t1.loadFromFile("res/mouse.jpg");
	t2.loadFromFile("res/cat.jpg");
	t3.loadFromFile("res/cheese.jpg");

	sf::Sprite s1(t1), s2(t2), s3(t3);

	//board setup, 0- nothing, 1- mouse, 2-cat, 3-cheese/end
	int board[8][8] = { 0 };
	board[0][0] = 1;
	board[5][5] = 3;
	board[0][2] = 2; board[5][1] = 2; board[6][1] = 2; 
	board[1][3] = 2;board[4][4] = 2;
	 board[6][6] = 2; board[6][7] = 2;
	//reward table
	int reward[8][8] = { -10 };
	reward[5][5] = 1000;
	reward[0][2] = -100;  reward[5][1] = -100; reward[6][1] = -100; 
	reward[1][3] = -100;  reward[4][4] = -100; 
	reward[6][6] = -100; reward[6][7] = -100;
	//mouse object
	Mouse m;

	//game loop
	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) window.close();
		}
		board[5][5] = 3;
		//set boardPos of mouse
		sf::Vector2i boardPos;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] == 1) boardPos = sf::Vector2i(i, j);
			}
		}

		//move mouse
		board[boardPos.x][boardPos.y] = 0;
		m.move(board, boardPos,reward);
		board[boardPos.x][boardPos.y] = 1;
		
		window.clear(sf::Color::White);
		
		//draw values for each space
		for (int i = 0, y = 0; i < 64; i++) {
			//combine qtable values
			float comb = m.qtable[0][i] + m.qtable[1][i] + m.qtable[2][i] + m.qtable[3][i];
			//convert to string
			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << comb;
			t.setString(ss.str());
			//calculate y value for drawing (implement alg later?)
			t.setPosition(sf::Vector2f((i % 8) * 80 + 5, floor(i / 8) * 80 + 5));
			window.draw(t);
			
		}
		//draw mouse, cat & cheese
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] == 1) {
					s1.setPosition(sf::Vector2f(i * 80 + 5, j * 80 + 5));
					window.draw(s1);
				}
				if (board[i][j] == 2) {
					s2.setPosition(sf::Vector2f(i * 80 + 5, j * 80 + 5));
					window.draw(s2);
				}
				if (board[i][j] == 3) {
					s3.setPosition(sf::Vector2f(i * 80 + 5, j * 80 + 5));
					window.draw(s3);
				}
			}
		}

		//draw lines
		for (int i = 0; i < 14; i++) {
			if(i < 7) line.setPosition(sf::Vector2f(0, 80 * (i + 1)));
			else line.setPosition(sf::Vector2f((i - 6) * 80, 0));
			if (i < 7) line.setRotation(0.f); else line.setRotation(90.f);

			window.draw(line);
		}
		window.display();
	}

	return 0;
}