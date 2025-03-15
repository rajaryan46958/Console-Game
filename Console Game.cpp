#include <iostream>

#include <conio.h>

#include <windows.h>

#include <vector>

#include <algorithm>

#include <cstdlib>

#include <ctime>

#include <fstream> // File handling for high score

using namespace std;

#define WIDTH 30

#define HEIGHT 20

// 4x4 Car Design

char car[4][4] = {

    { ' ', '', '', ' ' },

    { '', '', '', '' },

    { ' ', '', '', ' ' },

    { '', '', '', '' }

};




struct Car {

    int x, y;

};

vector<pair<int, int>> enemies;

pair<int, int> fuelPowerUp = {-1, -1}; // Fuel power-up position

Car player;

int score = 0, fuel = 100, speed = 120, highScore = 0;

bool gameOver = false;

// Move cursor for smooth rendering

void gotoxy(int x, int y) {

    COORD coord;

    coord.X = x;

    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}

// Set text color

void setColor(int color) {

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

}

// Hide console cursor

void hideCursor() {

    CONSOLE_CURSOR_INFO cursorInfo;

    cursorInfo.bVisible = false;

    cursorInfo.dwSize = 1;

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

}

// Load high score from file

void loadHighScore() {

    ifstream file("highscore.txt");

    if (file.is_open()) {

        file >> highScore;

        file.close();

    }

}

// Save high score to file

void saveHighScore() {

    if (score > highScore) {

        highScore = score;

        ofstream file("highscore.txt");

        if (file.is_open()) {

            file << highScore;

            file.close();

        }

    }

}

// Show game instructions

void showInstructions() {

    system("cls");

    setColor(11);

    cout << "==================================\n";

    cout << "        🚗 CAR RACING GAME        \n";

    cout << "==================================\n\n";

    setColor(15);

    cout << "Instructions:\n";

    cout << "➡ 'A' = Move Left | 'D' = Move Right\n";

    cout << "➡ Avoid enemies (▼)\n";

    cout << "➡ Fuel (⛽) runs out; collect '⛽' to refill\n";

    cout << "➡ Press 'X' to Exit\n";

    cout << "\nSelect Difficulty:\n";

    cout << "1 = Easy  |  2 = Medium  |  3 = Hard\n";

    char choice = _getch();

    if (choice == '1') speed = 140;

    else if (choice == '2') speed = 120;

    else speed = 100;

}

// Initialize game

void setup() {

    player.x = WIDTH / 2;

    player.y = HEIGHT - 5; // Car takes more space

    enemies.clear();

    fuelPowerUp = {-1, -1};

    score = 0;

    fuel = 100;

    system("cls");

    hideCursor();

}

// Draw track borders

void drawBorders() {

    setColor(8);

    for (int i = 0; i < HEIGHT; i++) {

        gotoxy(0, i);

        cout << "|";

        gotoxy(WIDTH, i);

        cout << "|";

    }

}




// Draw all game elements

void drawObjects() {

    // Draw Player Car

    setColor(10);

    for (int i = 0; i < 4; i++) {

        gotoxy(player.x, player.y + i);

        for (int j = 0; j < 4; j++) {

            cout << car[i][j];

        }

    }




    // Draw Enemies

    setColor(12);

    for (auto &e : enemies) {

        gotoxy(e.first, e.second);

        cout << "▼";

    }




    // Draw Fuel Power-Up

    if (fuelPowerUp.first != -1) {

        setColor(14);

        gotoxy(fuelPowerUp.first, fuelPowerUp.second);

        cout << "⛽";

    }

}




// Erase all elements before redrawing

void eraseObjects() {

    // Erase Player Car

    for (int i = 0; i < 4; i++) {

        gotoxy(player.x, player.y + i);

        cout << "    ";

    }




    // Erase Enemies

    for (auto &e : enemies) {

        gotoxy(e.first, e.second);

        cout << " ";

    }




    // Erase Fuel Power-Up

    if (fuelPowerUp.first != -1) {

        gotoxy(fuelPowerUp.first, fuelPowerUp.second);

        cout << " ";

    }

}




// Move enemies & power-ups

void updateObjects() {

    eraseObjects();



    for (auto &e : enemies) e.second++;




    enemies.erase(remove_if(enemies.begin(), enemies.end(),

                            [](pair<int, int> e) { return e.second >= HEIGHT; }),

                  enemies.end());




    if (rand() % 10 < 3) enemies.push_back({rand() % (WIDTH - 2) + 1, 0});




    // Spawn fuel power-up

    if (fuelPowerUp.first == -1 && rand() % 20 == 0) {

        fuelPowerUp = {rand() % (WIDTH - 2) + 1, 0};

    }




    // Move fuel power-up

    if (fuelPowerUp.first != -1) {

        fuelPowerUp.second++;

        if (fuelPowerUp.second >= HEIGHT) fuelPowerUp = {-1, -1};

    }

}




// Check collision

void checkCollision() {

    for (auto &e : enemies) {

        if (e.first >= player.x && e.first < player.x + 4 &&

            e.second >= player.y && e.second < player.y + 4) {

            gameOver = true;

        }

    }




    // Check if player collects fuel power-up

    if (fuelPowerUp.first >= player.x && fuelPowerUp.first < player.x + 4 &&

        fuelPowerUp.second >= player.y && fuelPowerUp.second < player.y + 4) {

        fuel = min(100, fuel + 20);

        fuelPowerUp = {-1, -1};

    }

}




// Display score & fuel

void displayStats() {

    setColor(11);

    gotoxy(WIDTH + 5, 2);

    cout << "Score: " << score << "  ";

    gotoxy(WIDTH + 5, 4);

    cout << "Fuel: " << fuel << "%   ";

    gotoxy(WIDTH + 5, 6);

    cout << "High Score: " << highScore << "  ";

}




// Player movement

void input() {

    if (_kbhit()) {

        char key = _getch();

        eraseObjects();

        if (key == 'a' || key == 'A') player.x = max(1, player.x - 2);

        if (key == 'd' || key == 'D') player.x = min(WIDTH - 5, player.x + 2);

        if (key == 'x' || key == 'X') gameOver = true;

    }

}




// Main game loop

void runGame() {

    while (!gameOver) {

        displayStats();

        input();

        updateObjects();

        drawBorders();

        drawObjects();

        checkCollision();

        Sleep(speed);

        if (score % 5 == 0) fuel--;

        if (fuel <= 0) gameOver = true;

        score++;

    }

}




// Game over screen

void showGameOver() {

    saveHighScore();

    system("cls");

    setColor(12);

    cout << "=========================\n";

    cout << "       GAME OVER!       \n";

    cout << "    Your Score: " << score << "   \n";

    cout << "    High Score: " <<highScore << "  \n";

    cout << "=========================\n";

    _getch();

}




int main() {

    srand(time(0));

    loadHighScore();

    showInstructions();

    setup();

    runGame();

    showGameOver();

    return 0;

}
