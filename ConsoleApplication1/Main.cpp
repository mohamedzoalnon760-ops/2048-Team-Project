#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib> 
#include <ctime>   

// Struct to hold game data
struct GameData {
    int board[4][4] = { 0 };
    int score = 0;
};

// --- [New Part: Random Number Generator] ---
// Function to find empty cells and place a 2 or 4
void addRandomNumber(GameData& game) {
    int emptyCells[16][2];
    int count = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game.board[i][j] == 0) {
                emptyCells[count][0] = i;
                emptyCells[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int index = rand() % count;
        int r = emptyCells[index][0];
        int c = emptyCells[index][1];
        // 90% chance for 2, 10% chance for 4
        game.board[r][c] = (rand() % 10 < 9) ? 2 : 4;
    }
}

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
}

void moveLeft(GameData& game) {
    for (int i = 0; i < 4; i++) {
        int temp[4] = { 0 };
        int index = 0;

        // 1. Shift non-zero elements to the left
        for (int j = 0; j < 4; j++) {
            if (game.board[i][j] != 0) {
                temp[index++] = game.board[i][j];
            }
        }

        // 2. Merge identical adjacent tiles
        for (int j = 0; j < 3; j++) {
            if (temp[j] != 0 && temp[j] == temp[j + 1]) {
                temp[j] *= 2;
                game.score += temp[j];
                temp[j + 1] = 0;
                j++; // Prevent double merging in one move
            }
        }

        // 3. Shift again after merging to fill gaps
        int newRow[4] = { 0 };
        index = 0;
        for (int j = 0; j < 4; j++) {
            if (temp[j] != 0) {
                newRow[index++] = temp[j];
            }
        }

        // 4. Update the game board row
        for (int j = 0; j < 4; j++) {
            game.board[i][j] = newRow[j];
        }
    }
}

void moveRight(GameData& game) {
    for (int i = 0; i < 4; i++) {
        int temp[4] = { 0 };
        int index = 3;

        // 1. Shift non-zero elements to the right
        for (int j = 3; j >= 0; j--) {
            if (game.board[i][j] != 0) {
                temp[index--] = game.board[i][j];
            }
        }

        // 2. Merge identical adjacent tiles
        for (int j = 3; j > 0; j--) {
            if (temp[j] != 0 && temp[j] == temp[j - 1]) {
                temp[j] *= 2;
                game.score += temp[j];
                temp[j - 1] = 0;
                j--; // Prevent double merging
            }
        }

        // 3. Re-shift to fill gaps after merging
        int newRow[4] = { 0 };
        index = 3;
        for (int j = 3; j >= 0; j--) {
            if (temp[j] != 0) {
                newRow[index--] = temp[j];
            }
        }

        // 4. Update the game board row
        for (int j = 0; j < 4; j++) {
            game.board[i][j] = newRow[j];
        }
    }
}

int main() {
    // Seed random number generator using current time
    srand(static_cast<unsigned>(time(0)));

    // Creating the window
    sf::RenderWindow window(sf::VideoMode(600, 600), "2048 Game - Team Project");

    GameData myGame;

    // Initialize board with two random tiles
    addRandomNumber(myGame);
    addRandomNumber(myGame);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                bool moved = false;

                // Handle movement and flag if a move was made
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) { moveUp(myGame); moved = true; }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) { moveDown(myGame); moved = true; }
                else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) { moveLeft(myGame); moved = true; }
                else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) { moveRight(myGame); moved = true; }

                if (moved) {
                    addRandomNumber(myGame); // Add new tile after each move

                    // Console visualization
                    system("cls");
                    std::cout << "Total Score: " << myGame.score << "\n\n";
                    for (int r = 0; r < 4; r++) {
                        for (int c = 0; c < 4; c++) {
                            std::cout << myGame.board[r][c] << "\t";
                        }
                        std::cout << "\n\n";
                    }
                }
            }
        }

        window.clear();
        // Drawing logic for SFML shapes will go here
        window.display();
    }
    return 0;
}