#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <string>

using namespace std;

void newBoard(short(&board)[9][9], short(&boardShown)[9][9], bool(&solved)[9][9], int difficulty);
void printBoard(short(&board)[9][9]);
void loadBoard(sf::RenderWindow& window, short(&board)[9][9], bool(&solved)[9][9], sf::Texture(&numbers)[10]);

int main()
{
    int x;
    int y;
    bool selected = false;

    sf::RenderWindow window(sf::VideoMode(598, 666), "Soduko");
    sf::Texture background;
    sf::Texture newEasy;
    sf::Texture newMedium;
    sf::Texture newHard;
    sf::Sprite sprite;
    sf::Texture numbers[10];
    sf::Texture numbersSelected[10];
    sf::Texture numbersBad[10];
    sf::Vector2f currPos;
    sf::Vector2f selectedPos;

    background.loadFromFile("images/background.png");
    newEasy.loadFromFile("images/easy.png");
    newMedium.loadFromFile("images/medium.png");
    newHard.loadFromFile("images/hard.png");
    numbers[0].loadFromFile("images/blank.png");

    for (int i = 1; i < 10; i++) {
        numbers[i].loadFromFile("images/" + to_string(i) + ".png");
    }

    for (int i = 0; i < 10; i++) {
        numbersSelected[i].loadFromFile("images/" + to_string(i) + "selected.png");
    }

    for (int i = 1; i < 10; i++) {
        numbersBad[i].loadFromFile("images/" + to_string(i) + "bad.png");
    }

    short board[9][9] = { {8,2,7,1,5,4,3,9,6},{9,6,5,3,2,7,1,4,8},{3,4,1,6,8,9,7,5,2},{5,9,3,4,6,8,2,7,1},{4,7,2,5,1,3,6,8,9},{6,1,8,9,7,2,4,3,5},{7,8,6,2,3,5,9,1,4},{1,5,4,7,9,6,8,2,3},{2,3,9,8,4,1,5,6,7} };
    short boardShown[9][9] = { {8,2,7,1,5,4,3,9,6},{9,6,5,3,2,7,1,4,8},{3,4,1,6,8,9,7,5,2},{5,9,3,4,6,8,2,7,1},{4,7,2,5,1,3,6,8,9},{6,1,8,9,7,2,4,3,5},{7,8,6,2,3,5,9,1,4},{1,5,4,7,9,6,8,2,3},{2,3,9,8,4,1,5,6,7} };
    bool solved[9][9];
    newBoard(board, boardShown, solved, 2);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();

            }

            x = event.mouseButton.x;
            y = event.mouseButton.y;
            currPos.x = 3 * (x - 6) / 196;
            currPos.y = 3 * (y - 6) / 196;

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (currPos.x < 9 && currPos.y < 9) {
                        if (selected && currPos == selectedPos) {
                            selected == false;

                        }
                        else if (selected) {
                            selectedPos = currPos;

                        }
                        else {
                            selected = true;
                            selectedPos = currPos;

                        }
                    }
                }
            }
        }

        window.clear();

        sprite.setTexture(background);
        sprite.setPosition(sf::Vector2f(0, 0));
        window.draw(sprite);

        sprite.setTexture(newEasy);
        sprite.setPosition(sf::Vector2f(201, 600));
        window.draw(sprite);

        sprite.setTexture(newMedium);
        sprite.setPosition(sf::Vector2f(267, 600));
        window.draw(sprite);

        sprite.setTexture(newHard);
        sprite.setPosition(sf::Vector2f(333, 600));
        window.draw(sprite);

        loadBoard(window, board, solved, numbers);

        if (selected) {
            sprite.setPosition(sf::Vector2f((selectedPos.x * 65) + (selectedPos.x / 3) + 6, (selectedPos.y * 65) + (selectedPos.y / 3) + 6));
            sprite.setTexture(numbersSelected[boardShown[(int)selectedPos.x][(int)selectedPos.y]]);

            window.draw(sprite);
        }

        window.display();
    }
}

void loadBoard(sf::RenderWindow& window, short(&board)[9][9], bool(&solved)[9][9], sf::Texture(&numbers)[10]) {
    sf::Sprite sprite[9][9];

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (solved[i][j]) {
                sprite[i][j].setTexture(numbers[board[i][j]]);
                sprite[i][j].setPosition(sf::Vector2f((i * 65) + (i / 3) + 6, (j * 65) + (j / 3) + 6));
                window.draw(sprite[i][j]);

            }
            else {
                sprite[i][j].setTexture(numbers[0]);
                sprite[i][j].setPosition(sf::Vector2f((i * 65) + (i / 3) + 6, (j * 65) + (j / 3) + 6));
                window.draw(sprite[i][j]);

            }
        }
    }
}

void newBoard(short(&board)[9][9], short(&boardShown)[9][9], bool(&solved)[9][9], int difficulty) {
    char temp;
    char swap_9;
    char swap_3;
    char randX;
    char randY;
    float difficultyScale;
    unsigned const char STARTING_REVEALED = 17;

    // set random numm generation base off time
    time_t curTime;
    time(&curTime);
    srand(rand() * curTime);

    // make 100 swaps to create a new board
    for (int i = 0; i < 100; i++) {
        swap_9 = rand() % 9;   // Choose a row and column
        swap_3 = rand() % 3;   // Choose row and column within each 3x3 box to swap with

        // swap corresponding row (swaps are only made with the 3x3 box)
        for (int j = 0; j < 9; j++) {
            temp = board[swap_9][j];
            board[swap_9][j] = board[((swap_9 / 3) * 3) + swap_3][j];
            board[((swap_9 / 3) * 3) + swap_3][j] = temp;
        }

        // swap corresponding column (swaps are only made with the 3x3 box)
        for (int j = 0; j < 9; j++) {
            temp = board[j][swap_9];
            board[j][swap_9] = board[j][((swap_9 / 3) * 3) + swap_3];
            board[j][((swap_9 / 3) * 3) + swap_3] = temp;
        }
    }

    // initialize all solved tiles to false
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            solved[i][j] = false;
        }
    }

    // level of difficulty will determine the scale for the number of starting revealed numbers
    if (difficulty <= 1) {
        difficultyScale = 2.0;

    }
    else if (difficulty >= 3) {
        difficultyScale = 1.0;

    }
    else {
        difficultyScale = 1.5;

    }

    // loop through and reveal tiles 
    for (int i = 0; i < STARTING_REVEALED * difficultyScale; i++) {
        randX = rand() % 9;
        randY = rand() % 9;

        // if number is already revealed, ignore this iteration
        if (solved[randX][randY]) {
            i--;

            // else reveal number
        }
        else {
            solved[randX][randY] = true;

        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (solved[i][j]) {
                boardShown[i][j] = board[i][j];

            }
            else {
                boardShown[i][j] = 0;
            }
        }
    }
}

void printBoard(short(&board)[9][9]) { // FIXME: Print board function for testing, remove later
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j];
        }

        cout << endl;
    }
}