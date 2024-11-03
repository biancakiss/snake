#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>      
#include <windows.h>  
#include <time.h> //for seeding the random number generator
#define MAXSNAKESIZE 100
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 25
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //handle to the console for setting the cursor position
COORD CursorPosition; //structure to hold the cursor position

//function to set the cursor position in the console
void gotoxy(int x, int y) {
    CursorPosition.X = x; 
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition); //moves the cursor to the specified position
}

void printScore(int score) {
    gotoxy(0, 0); 
    cout << "Score: " << score; 
}

class Point {
private:
    int x, y; 
public:
    Point() : x(10), y(10) {} 
    Point(int x, int y) : x(x), y(y) {} 

    void SetPoint(int x, int y) { this->x = x; this->y = y; }
    int GetX() { return x; } 
    int GetY() { return y; }

    void MoveUp() { y--; }
    void MoveDown() { y++; }
    void MoveLeft() { x--; }
    void MoveRight() { x++; }

    void Draw() {
        gotoxy(x, y); //moves cursor to the point's position
        cout << "*";  //draws the point (part of the snake) as *
    }

    //erasing the point from the console
    void Erase() {
        gotoxy(x, y); 
        cout << " ";  //overwrites the point with a space
    }

    //copies the position of this point to another point
    void CopyPos(Point* p) {
        p->x = x; 
        p->y = y; 
    }
};

class Snake {
private:
    Point* cell[MAXSNAKESIZE]; 
    int size;                   
    char dir;                   
    Point fruit;               
    bool isGameOver;            
    int score;                  

public:
    Snake() : size(1), dir('d'), isGameOver(false), score(1) {
        cell[0] = new Point(20, 20); 
        for (int i = 1; i < MAXSNAKESIZE; i++) 
            cell[i] = nullptr;
        fruit.SetPoint(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT); //setting initial fruit position randomly
    }

    void AddCell() {
        int lastX = cell[size - 1]->GetX(); 
        int lastY = cell[size - 1]->GetY(); 
        cell[size++] = new Point(lastX, lastY); 
    }

    void TurnUp() { if (dir != 's') dir = 'w'; }
    void TurnDown() { if (dir != 'w') dir = 's'; }
    void TurnLeft() { if (dir != 'd') dir = 'a'; }
    void TurnRight() { if (dir != 'a') dir = 'd'; }

    void Move() {
        if (isGameOver) return; 
        cell[size - 1]->Erase(); 

        for (int i = size - 1; i > 0; i--) 
            cell[i - 1]->CopyPos(cell[i]); //copy the position from the next cell

        switch (dir) {
        case 'w': cell[0]->MoveUp(); break;   
        case 's': cell[0]->MoveDown(); break;   
        case 'a': cell[0]->MoveLeft(); break;   
        case 'd': cell[0]->MoveRight(); break;  
        }

        //screen edges
        int x = cell[0]->GetX(); 
        int y = cell[0]->GetY();
        if (x >= SCREEN_WIDTH) cell[0]->SetPoint(0, y);
        else if (x < 0) cell[0]->SetPoint(SCREEN_WIDTH - 1, y); 
        if (y >= SCREEN_HEIGHT) cell[0]->SetPoint(x, 0); 
        else if (y < 0) cell[0]->SetPoint(x, SCREEN_HEIGHT - 1); 

        //game over condition
        for (int i = 1; i < size; i++) {
            if (cell[0]->GetX() == cell[i]->GetX() && cell[0]->GetY() == cell[i]->GetY()) {
                isGameOver = true; 
                break;
            }
        }

        if (isGameOver) {
            gotoxy(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); //moving the cursor to the middle of the screen
            cout << "Game Over!"; 
            return; 
        }

        //when the snake eats a fruit
        if (cell[0]->GetX() == fruit.GetX() && cell[0]->GetY() == fruit.GetY()) {
            AddCell(); 
            fruit.SetPoint(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT); //setting new fruit position
            score++; 
        }

        for (int i = 0; i < size; i++) {
            cell[i]->Draw(); //drawing each segment of the snake
        }
        fruit.Draw(); //drawing the fruit
        printScore(score);

        Sleep(100);  //delay to control the game speed
    }
};

int main() {
    srand(static_cast<unsigned>(time(0))); //seeding the random number generator with the current time
    Snake snake; 
    char op = 'l'; //variable to hold the current operation 

    do {
        if (_kbhit()) { //check for keyboard input
            op = _getch(); //get the input character
        }
        switch (op) {
        case 'w': case 'W': snake.TurnUp(); break; 
        case 's': case 'S': snake.TurnDown(); break; 
        case 'a': case 'A': snake.TurnLeft(); break;
        case 'd': case 'D': snake.TurnRight(); break; 
        }
        snake.Move(); 
    } while (op != 'e'); //press 'e' to exit the game

    return 0; 
}