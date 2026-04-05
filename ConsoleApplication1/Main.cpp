#include <SFML/Graphics.hpp>
#include <iostream>

// Struct to hold game data
struct GameData {
    int board[4][4] = { 0 };
    int score = 0;
};

// Function for upward movement
void moveUp(GameData& game) {
    for (int j = 0; j < 4; j++) {
        for (int i = 1; i < 4; i++) {
            if (game.board[i][j] != 0) {
                int k = i;
                while (k > 0 && game.board[k - 1][j] == 0) {
                    game.board[k - 1][j] = game.board[k][j];
                    game.board[k][j] = 0;
                    k--;
                }
                if (k > 0 && game.board[k - 1][j] == game.board[k][j]) {
                    game.board[k - 1][j] *= 2;
                    game.score += game.board[k - 1][j];
                    game.board[k][j] = 0;
                }
            }
        }
    }
    std::cout << "Action: Move Up" << std::endl;
}

// Function for downward movement
void moveDown(GameData& game) {
    for (int j = 0; j < 4; j++) {
        for (int i = 2; i >= 0; i--) {
            if (game.board[i][j] != 0) {
                int k = i;
                while (k < 3 && game.board[k + 1][j] == 0) {
                    game.board[k + 1][j] = game.board[k][j];
                    game.board[k][j] = 0;
                    k++;
                }
                if (k < 3 && game.board[k + 1][j] == game.board[k][j]) {
                    game.board[k + 1][j] *= 2;
                    game.score += game.board[k + 1][j];
                    game.board[k][j] = 0;
                }
            }
        }
    }
    std::cout << "Action: Move Down" << std::endl;
}

int main() {
    // Creating the window
    sf::RenderWindow window(sf::VideoMode(600, 600), "2048 Game - Team Project");

    // Creating game object
    GameData myGame;

    // Initial value for testing
    myGame.board[3][0] = 2;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handling arrow keys
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    moveUp(myGame);
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    moveDown(myGame);
                }
            }
        }

        window.clear();
        // Drawing logic will be added here later
        window.display();
    }
    return 0;
}