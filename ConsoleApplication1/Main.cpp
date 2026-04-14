#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib> 
#include <ctime>   
#include <string>  
#include <map>


struct GameData {
    int board[4][4] = { 0 };
    int score = 0;
};

//  لون المربع بناءً على قيمته
sf::Color getTileColor(int value) {
    switch (value) {
    case 2:    return sf::Color(238, 228, 218);
    case 4:    return sf::Color(237, 224, 200);
    case 8:    return sf::Color(242, 177, 121);
    case 16:   return sf::Color(245, 149, 99);
    case 32:   return sf::Color(246, 124, 95);
    case 64:   return sf::Color(246, 94, 59);
    case 128:  return sf::Color(237, 207, 114);
    case 256:  return sf::Color(237, 204, 97);
    case 512:  return sf::Color(237, 200, 80);
    case 1024: return sf::Color(237, 197, 63);
    case 2048: return sf::Color(237, 194, 46);
    default:   return sf::Color(205, 193, 180); // لون المربعات الفارغة
    }
}

sf::Color getTextColor(int value) {
    return (value <= 4) ? sf::Color(119, 110, 101) : sf::Color::White;
}

// التحقق من الفوز
bool checkWin(const GameData& game) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (game.board[i][j] == 2048) return true;
    return false;
}


bool checkGameOver(const GameData& game) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (game.board[i][j] == 0) return false;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            if (game.board[i][j] == game.board[i][j + 1]) return false;

    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 3; i++)
            if (game.board[i][j] == game.board[i + 1][j]) return false;

    return true;
}


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
        game.board[r][c] = (rand() % 10 < 9) ? 2 : 4;
    }
}

// دوال الحركة
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
        int temp[4] = { 0 }, index = 0;
        for (int j = 0; j < 4; j++) if (game.board[i][j] != 0) temp[index++] = game.board[i][j];
        for (int j = 0; j < 3; j++) {
            if (temp[j] != 0 && temp[j] == temp[j + 1]) {
                temp[j] *= 2; game.score += temp[j]; temp[j + 1] = 0; j++;
            }
        }
        int newRow[4] = { 0 }; index = 0;
        for (int j = 0; j < 4; j++) if (temp[j] != 0) newRow[index++] = temp[j];
        for (int j = 0; j < 4; j++) game.board[i][j] = newRow[j];
    }
}

void moveRight(GameData& game) {
    for (int i = 0; i < 4; i++) {
        int temp[4] = { 0 }, index = 3;
        for (int j = 3; j >= 0; j--) if (game.board[i][j] != 0) temp[index--] = game.board[i][j];
        for (int j = 3; j > 0; j--) {
            if (temp[j] != 0 && temp[j] == temp[j - 1]) {
                temp[j] *= 2; game.score += temp[j]; temp[j - 1] = 0; j--;
            }
        }
        int newRow[4] = { 0 }; index = 3;
        for (int j = 3; j >= 0; j--) if (temp[j] != 0) newRow[index--] = temp[j];
        for (int j = 0; j < 4; j++) game.board[i][j] = newRow[j];
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    sf::RenderWindow window(sf::VideoMode(600, 720), "2048 Game - Full Project");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("Clear-Sans.Bold.ttf")) {
        std::cout << "Font not found! Place 'Clear-Sans.Bold.ttf' in the folder." << std::endl;
        return -1;
    }

    GameData myGame;
    addRandomNumber(myGame);
    addRandomNumber(myGame);

    sf::Text scoreDisplay;
    scoreDisplay.setFont(font);
    scoreDisplay.setCharacterSize(30);
    scoreDisplay.setFillColor(sf::Color(119, 110, 101));
    scoreDisplay.setPosition(20, 630);

    const float TILE_SIZE = 130.0f;
    const float PADDING = 16.0f;
    sf::RectangleShape tileShape;
    sf::Text tileText;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                bool moved = false;
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) { moveUp(myGame); moved = true; }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) { moveDown(myGame); moved = true; }
                else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) { moveLeft(myGame); moved = true; }
                else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) { moveRight(myGame); moved = true; }

                if (moved) addRandomNumber(myGame);
            }
        }

        window.clear(sf::Color(187, 173, 160));
        scoreDisplay.setString("Total Score: " + std::to_string(myGame.score));
        window.draw(scoreDisplay);

        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                int val = myGame.board[r][c];
                float xPos = PADDING + c * (TILE_SIZE + PADDING);
                float yPos = PADDING + r * (TILE_SIZE + PADDING);

                tileShape.setSize({ TILE_SIZE, TILE_SIZE });
                tileShape.setPosition(xPos, yPos);
                tileShape.setFillColor(getTileColor(val));
                window.draw(tileShape);

                if (val != 0) {
                    tileText.setFont(font);
                    tileText.setString(std::to_string(val));
                    tileText.setCharacterSize(val < 100 ? 45 : (val < 1000 ? 35 : 28));
                    tileText.setFillColor(getTextColor(val));

                    sf::FloatRect textRect = tileText.getLocalBounds();
                    tileText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    tileText.setPosition(xPos + TILE_SIZE / 2.0f, yPos + TILE_SIZE / 2.0f);
                    window.draw(tileText);
                }
            }
        }

        if (checkGameOver(myGame)) {
            sf::RectangleShape overlay(sf::Vector2f(600, 720));
            overlay.setFillColor(sf::Color(255, 255, 255, 120));
            window.draw(overlay);

            sf::Text loseText;
            loseText.setFont(font);
            loseText.setString("GAME OVER!");
            loseText.setCharacterSize(60);
            loseText.setFillColor(sf::Color::Red);
            loseText.setOrigin(loseText.getLocalBounds().width / 2.0f, loseText.getLocalBounds().height / 2.0f);
            loseText.setPosition(300, 300);
            window.draw(loseText);
        }

        window.display();
    }
    return 0;
}