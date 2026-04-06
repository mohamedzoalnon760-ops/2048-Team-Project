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
    std ::cout << "Action: Move Down" << std::endl;
}
void moveLeft(GameData& game)
{
    for (int i = 0; i < 4; i++)
    {
        int temp[4] = { 0 };
        int index = 0;

        // 1. ضغط (تشيل الأصفار)
        for (int j = 0; j < 4; j++)
        {
            if (game.board[i][j] != 0)
            {
                temp[index++] = game.board[i][j];
            }
        }

        // 2. دمج
        for (int j = 0; j < 3; j++)
        {
            if (temp[j] != 0 && temp[j] == temp[j + 1])
            {
                temp[j] *= 2;
                game.score += temp[j];
                temp[j + 1] = 0;
            }
        }

        // 3. ضغط تاني
        int newRow[4] = { 0 };
        index = 0;
        for (int j = 0; j < 4; j++)
        {
            if (temp[j] != 0)
            {
                newRow[index++] = temp[j];
            }
        }

        // 4. رجوع للبورد
        for (int j = 0; j < 4; j++)
        {
            game.board[i][j] = newRow[j];
        }
    }
}

void moveRight(GameData& game)
{
    for (int i = 0; i < 4; i++)
    {
        int temp[4] = { 0 };
        int index = 3;

        // 1. ضغط من اليمين
        for (int j = 3; j >= 0; j--)
        {
            if (game.board[i][j] != 0)
            {
                temp[index--] = game.board[i][j];
            }
        }

        // 2. دمج
        for (int j = 3; j > 0; j--)
        {
            if (temp[j] != 0 && temp[j] == temp[j - 1])
            {
                temp[j] *= 2;
                game.score += temp[j];
                temp[j - 1] = 0;
            }
        }

        // 3. ضغط تاني
        int newRow[4] = { 0 };
        index = 3;
        for (int j = 3; j >= 0; j--)
        {
            if (temp[j] != 0)
            {
                newRow[index--] = temp[j];
            }
        }

        // 4. رجوع للبورد
        for (int j = 0; j < 4; j++)
        {
            game.board[i][j] = newRow[j];
        }
    }
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

            // arrow keys
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    moveUp(myGame);
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    moveDown(myGame);
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    moveLeft(myGame);
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    moveRight(myGame);
                }
            }
        }

        window.clear();
        // Drawing logic will be added here later
        window.display();
    }
    return 0;
}