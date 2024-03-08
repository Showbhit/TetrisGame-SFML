#include <SFML/Graphics.hpp>
#include <time.h>
#include<iostream>
using namespace sf;

const int rows = 15, cols = 15;

int grid[rows][cols] = { 0 };


class coordinates {
public: 
	int x, y;
};

coordinates curr[4], prev[4];

coordinates tetriminoes[7][4] = {
	{{1,0},{1,1},{1,2},{1,3}},// i
	{{0,1},{1,1},{1,2},{2,2}},
	{{0,1},{1,1},{2,1},{1,2}},
	{{0,0},{1,0},{1,1},{1,2}},
	{{0,1},{1,1},{0,2},{1,2}},
	{{0,2},{1,1},{1,2},{2,1}},
	{{1,0},{1,1},{1,2},{2,2}}
};

bool validMovement() {
	for (int i = 0; i < 4; i++) {
		if (curr[i].x < 0 || curr[i].x >= cols || curr[i].y >= rows)
			return 0;
		else if (grid[curr[i].y][curr[i].x])
			return 0;
	}
	return 1;
}

void restartGame() {
	// Reset game variables
	int changeX = 0;
	bool rotate = false;
	float timer = 0, delay = 0.5;

	// Clear the grid
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid[i][j] = 0;
		}
	}

	// Set initial conditions for the tetrimino
	int colourNum = 1 + rand() % 7;
	int n = rand() % 7;

	for (int i = 0; i < 4; i++) {
		curr[i].x = tetriminoes[n][i].x;
		curr[i].y = tetriminoes[n][i].y;
	}

}
void clearRow(int row) {
	for (int k = row; k > 0; k--) {
		for (int j = 0; j < cols; j++) {
			grid[k][j] = grid[k - 1][j];
		}
	}
}
bool rowFilled(int row) {
	for (int j = 0; j < cols; j++) {
		if (grid[row][j] == 0) {
			return false;
		}
	}
	return true;
}


void updateScore(int &score) {
	for (int i = rows - 1; i >= 0; i--) {
		if (rowFilled(i)) {
			score += 10;
			clearRow(i);
		}		
	}
}

int main() {
	srand(time(0));

	int score = 0;
	
	RenderWindow window(VideoMode(800, 500), "SFML: TETRIS GAME! ");

	Texture t1, t2, t3;
	t1.loadFromFile("tiles.png");
	t2.loadFromFile("bgm.png");
	t3.loadFromFile("");

	Sprite block(t1), background(t2), outline(t3);

	int changeX = 0, colourNum = 0;
	bool rotate = 0, restart = 0;
	float timer = 0, delay = 0.5;

	Clock clock;

	RectangleShape restartButton(Vector2f(90, 20));
	restartButton.setFillColor(Color::Black);
	restartButton.setPosition(550, 10);

	Font font;
	font.loadFromFile("arial.ttf"); // Change the font path accordingly

	Text buttonText("RESTART", font, 15);
	buttonText.setPosition(550, 10);

	Text scoreText("SCORE: ", font, 15);
	scoreText.setPosition(650, 10);

	while (window.isOpen()) {

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		if (restart) {
			restartGame();
			restart = false;
			score = 0;
		}

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();
			if (e.KeyPressed && e.key.code == Keyboard::Escape)
				window.close();//
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Up)
					rotate = true;
				else if (e.key.code == Keyboard::Left)
					changeX = -1;
				else if (e.key.code == Keyboard::Right)
					changeX = 1;
				else if (e.key.code == Keyboard::R)
					restart = true;
			}
			if (e.type == Event::MouseButtonPressed) {
				if (e.mouseButton.button == Mouse::Left) {
					Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
					if (restartButton.getGlobalBounds().contains(mousePos)) {
						restart = true;
					}
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05;

		//
		

		// downwards movement
		
		for (int i = 0; i < 4; i++) {
			prev[i] = curr[i];
			curr[i].x += changeX;
		}
		if (!validMovement()) {
			for (int i = 0; i < 4; i++)
				curr[i] = prev[i];
		}
		
		// rotation of tetriminoes 

		if (rotate) {
			coordinates p = curr[1];
			for (int i = 0; i < 4; i++) {

				int x = curr[i].y - p.y;
				int y = curr[i].x - p.x;

				curr[i].x = p.x - x;
				curr[i].y = p.y + y;
			}
			if (!validMovement()) {
				for (int i = 0; i < 4; i++)
					curr[i] = prev[i];
			}
		}

		// tick
		
		if (timer > delay) {
			for (int i = 0; i < 4; i++) {
				prev[i] = curr[i];
				curr[i].y += 1;
			}

			if (!validMovement()) {
				for (int i = 0; i < 4; i++)
					grid[prev[i].y][prev[i].x] = colourNum;

				colourNum = 1 + rand() % 7;
				int n = rand() % 7;

				for (int i = 0; i < 4; i++) {
					curr[i].x = tetriminoes[n][i].x;
					curr[i].y = tetriminoes[n][i].y;
				}
			}
			timer = 0;
		}

		// check full lines and update the grid!

		/*int k = rows - 1;
		for (int i = rows - 1; i > 0; i--) {
			int count = 0;
			for (int j = 0; j < cols; j++) {
				if (grid[i][j])
					count++;
				grid[k][j] = grid[i][j];
			}
			if (count < cols)
				k--;
		}*/

		changeX = 0; rotate = 0; delay = 0.5;

		updateScore(score);
		// Update the score text
		scoreText.setString("SCORE: " + std::to_string(score));
		// draw the window

		window.clear(Color::White);
		window.draw(background);

		window.draw(restartButton);
		window.draw(buttonText);

		window.draw(scoreText);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (grid[i][j] == 0)
					continue;
				block.setTextureRect(IntRect(grid[i][j] * 18, 0, 18, 18));
				block.setPosition(j * 18, i * 18);
				block.move(200, 10);
				window.draw(block);
			}
		}
		for (int i = 0; i < 4; i++) {
			block.setTextureRect(IntRect(colourNum * 18, 0, 18, 18));
			block.setPosition(curr[i].x * 18, curr[i].y * 18);
			block.move(200, 10);
			window.draw(block);
		}

		window.draw(outline);
		window.display();
		
	}
	return 0;
}
