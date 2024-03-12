#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

const int rows = 20, cols = 20; // Game Area - Grid 

int grid[rows][cols] = { 0 }, score = 0; // Initializing the grid

float delay = 0.5; // Time difference between consecutive blocks 

class coordinates {
public: 
	int x, y;
};

coordinates curr[4], prev[4];  // Two arrays one to store the current coordinates and the other to store the previous one

coordinates tetriminoes[7][4] = {
	{{1,0},{1,1},{1,2},{1,3}}, // I
	{{0,1},{1,1},{1,2},{2,2}}, // S
	{{0,1},{1,1},{2,1},{1,2}}, // T
	{{0,0},{1,0},{1,1},{1,2}}, // L
	{{0,1},{1,1},{0,2},{1,2}}, // O
	{{0,2},{1,1},{1,2},{2,1}}, // Z
	{{1,0},{1,1},{1,2},{2,2}}  // J
};									// Giving Shapes to the blocks using the coordinates 


// Function for checking the height of the figure to check for Game over condition
int getHeight() {
	int height = 0;
	for (int j = rows-1; j>= 0; j--) {
		for (int i = 0; i <= cols-1; i++) {
			if (grid[j][i]) {
				height++;
				break;
			}
		}
	}
	return height; // If height equals to the number of rows then GAME OVER! 
}

// To check the movements allowed to a block 
// Whether it is in the given space or it is preoccupied or not! 
bool validMovement() {
	for (int i = 0; i < 4; i++) {
		if (curr[i].x < 0 || curr[i].x >= cols || curr[i].y >= rows)
			return 0;
		else if (grid[curr[i].y][curr[i].x])
			return 0;
	}
	return 1;
}


// Restarting the game with resetting the game variables 
void restartGame() {
	// Reset game variables
	int changeX = 0;
	bool rotate = false;
	float timer = 0, delay = 0.5;

	score = 0;
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

// Clearing the row, which is provided to make way for other blocks
// Interchanging it with the upper row 
void clearRow(int row) {
	for (int k = row; k > 0; k--) {
		for (int j = 0; j < cols; j++) {
			grid[k][j] = grid[k - 1][j];
		}
	}
}


bool rowFilled(int row) { // Function to check whether the row provided is filled or not 
	for (int j = 0; j < cols; j++) {
		if (grid[row][j] == 0) {
			return false;
		}
	}
	return true;
}


void updateScore(int &score, float &delay) { // It updates the score after checking if the row is filled or not
	for (int i = rows - 1; i >= 0; i--) {
		if (rowFilled(i)) {
			score += 10;
			clearRow(i);
			delay *= 0.9;
		}		
	}
}

int main() {
	//Gives random time
	srand(time(0));
	Clock clock;

	RenderWindow window(VideoMode(800, 500), "SFML: TETRIS GAME! "); // Creates a window 

	Texture t1, t2, t3; 
	t1.loadFromFile("tiles.png");
	t2.loadFromFile("bgm.png");
	t3.loadFromFile("frame.png");

	Sprite block(t1), background(t2), outline(t3);

	int changeX = 0, colourNum = 1 + rand() % 7, gameOverTime = 0; // Declaring Game variables 
	bool rotate = 0, restart = 0, paused = 0;
	float timer = 0;

	outline.setPosition(177,58); // Setting outline position
	
	RectangleShape restartButton(Vector2f(90, 20)); // Restart Button
	restartButton.setFillColor(Color::Black);
	restartButton.setPosition(450, 10);

	Font font; // Font for the written text 
	font.loadFromFile("arial.ttf"); 

	Text buttonText("RESTART", font, 17); 
	buttonText.setPosition(450, 10);

	Text scoreText("SCORE: ", font, 17); // Score text
	scoreText.setPosition(550, 10);

	Music backgroundMusic; // Background Music 
	if (!backgroundMusic.openFromFile("bgm1.mp3"))
		return -1; // error loading background music

	backgroundMusic.play();
	backgroundMusic.setVolume(0); // set volume to 50%
	backgroundMusic.setLoop(true);

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
		while (window.pollEvent(e)) { // Key events 
			if (e.type == Event::Closed) {
				window.close(); backgroundMusic.stop();
			}
			if (e.KeyPressed && e.key.code == Keyboard::Escape) {
				window.close(); backgroundMusic.stop();
			}
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Up)
					rotate = true;
				else if (e.key.code == Keyboard::Left)
					changeX = -1;
				else if (e.key.code == Keyboard::Right)
					changeX = 1;
				else if (e.key.code == Keyboard::R)
					restart = true;
				else if (e.key.code == Keyboard::Space) {
					paused = !paused; 
					if (paused)
						backgroundMusic.pause();
					else
						backgroundMusic.play();
				} 
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
		if (!paused && Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05;

		//
		

		// Downwards movement
		if(!paused){
		for (int i = 0; i < 4; i++) {
			prev[i] = curr[i];
			curr[i].x += changeX;
		}
		}
		if (!validMovement()) {
			for (int i = 0; i < 4; i++)
				curr[i] = prev[i];
		}
		
		// Rotation of tetriminoes 

		if (!paused && rotate) {
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
		
		// Generating Game 

		if (!paused && timer > delay) {
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
			if (getHeight() + 2 >= rows || !validMovement) {
				restartGame();
			}
			timer = 0;
		}

		changeX = 0; rotate = 0; delay = 0.5;
		updateScore(score, delay);
		// Update the score text
		scoreText.setString("SCORE: " + std::to_string(score));
		// draw the window

		window.clear(Color::White); // Clear window 

		window.draw(background); // Drawing on window 
		window.draw(outline);
		window.draw(restartButton);
		window.draw(buttonText);
		window.draw(scoreText);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (grid[i][j] == 0)
					continue;
				block.setTextureRect(IntRect(grid[i][j] * 18, 0, 18, 18));
				block.setPosition(j * 18, i * 18);
				block.move(200, 79);
				window.draw(block);
			}
		}
		for (int i = 0; i < 4; i++) {
			block.setTextureRect(IntRect(colourNum * 18, 0, 18, 18));
			block.setPosition(curr[i].x * 18, curr[i].y * 18);
			block.move(200, 79);
			window.draw(block);
		}

		
		window.display(); // Display the window 
		
	}
	return 0;
}
