#include <iostream>
#include <Windows.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
using namespace std;

enum Colors { BLACK, DARKBLUE, DARKGREEN, DARKCYAN, DARKRED, DARKMAGENTA, DARKYELLOW, GREY, DARKGREY, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE };
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
enum Directions { LEFT = 75, RIGHT = 77, DOWN = 80, UP = 72 };
enum Objects { HALL = 0, WALL = 1, COIN = 2, ENEMY = 3, HEAL = 4 };

void SetCursor(int x, int y, Colors color, string text)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(h, pos);
    SetConsoleTextAttribute(h, color);
    cout << text;
    cout << "\n";
}

void initializeConsole()
{
    SetConsoleOutputCP(CP_UTF8);
    system("title Maze");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(0));

    CONSOLE_CURSOR_INFO info;
    info.bVisible = false;
    info.bVisible = 1;
    SetConsoleCursorInfo(h, &info);
}

void generateMaze(int maze[][60], const int height, const int width)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // GENERATION
            maze[y][x] = rand() % 5;

            if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
                maze[y][x] = WALL;

            if (x == 0 && y == 2 || x == 1 && y == 2 || x == 2 && y == 2 ||
                x == width - 1 && y == height - 3)
                maze[y][x] = 0;

            if (maze[y][x] == 3)
            {
                int probability = rand() % 10;
                if (probability != 0)
                {
                    maze[y][x] = 0;
                }
            }
            if (maze[y][x] == 4)
            {
                int probability = rand() % 10;
                if (probability != 0)
                {
                    maze[y][x] = 0;
                }
            }
        }
    }
}

void printMaze(int maze[][60], const int height, const int width)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (maze[y][x] == HALL)
            {
                SetCursor(x, y, BLACK, " ");
            }
            else if (maze[y][x] == WALL)
            {
                SetCursor(x, y, DARKBLUE, "X");
            }
            else if (maze[y][x] == COIN)
            {
                SetCursor(x, y, YELLOW, ".");
            }
            else if (maze[y][x] == ENEMY)
            {
                SetCursor(x, y, RED, "*");
            }
            else if (maze[y][x] == HEAL)
            {
                SetCursor(x, y, RED, "I");
            }
            else
            {
                cout << maze[y][x];
            }
        }
        cout << "\n";
    }
}

void gameLoop(int maze[][60], const int height, const int width)
{
    COORD position;
    position.X = 0;
    position.Y = 2;

    SetCursor(position.X, position.Y, MAGENTA, "0");

    int coins = 0;
    int HealthPoints = 100;
    int enemyDAMAGE = 25;
    int medkitHEAL = 25;

    while (true)
    {
        // CONTROL
        int code = _getch();
        if (code == 224)
        {
            code = _getch();
        }
        SetConsoleCursorPosition(h, position);
        cout << " ";
        if (code == RIGHT and maze[position.Y][position.X + 1] != 1)
            position.X++;
        if (code == LEFT and maze[position.Y][position.X - 1] != 1)
            position.X--;
        if (code == UP and maze[position.Y - 1][position.X] != 1)
            position.Y--;
        if (code == DOWN and maze[position.Y + 1][position.X] != 1)
            position.Y++;
        SetCursor(position.X, position.Y, MAGENTA, "0");

        // LABYRINTH COMPLETION

        if (position.X == width - 1 and
            position.Y == height - 3)
        {
            MessageBoxA(0, "Вы прошли лабиринт", "победа", MB_OK);
            break;
        }
        if (maze[position.Y][position.X] == COIN)
        {
            // COIN ADD

            coins++;
            maze[position.Y][position.X] = HALL;
            COORD coins_info;
            SetCursor(width + 2, 1, YELLOW, "COINS:");
            SetCursor(width + 8, 1, YELLOW, to_string(coins));
        }
        if (maze[position.Y][position.X] == ENEMY or maze[position.Y][position.X] == HEAL)
        {
            // HP REMOVE

            if (maze[position.Y][position.X] == ENEMY)
            {
                HealthPoints = HealthPoints - enemyDAMAGE;
                maze[position.Y][position.X] = HALL;
            }
            if (maze[position.Y][position.X] == HEAL)
            {
                HealthPoints = HealthPoints + medkitHEAL;
                maze[position.Y][position.X] = HALL;
                if (HealthPoints > 100)
                {
                    HealthPoints = 100;
                }
            }
            COORD HP_info;
            SetCursor(width + 2, 2, RED, "HEALTH:");
            SetCursor(width + 9, 2, RED, to_string(HealthPoints));
            // LOSE SYSTEM
            if (HealthPoints < 5)
            {
                system("cls");
                cout << "lose (your hp is gone)\n\n";
                Sleep(INFINITE);
            }

        }

    }

    system("cls");
    cout << "win\n\n";
    Sleep(INFINITE);
}

int main()
{
    initializeConsole();

    const int width = 60;
    const int height = 25;
    int maze[25][60] = {};

    generateMaze(maze, height, width);
    printMaze(maze, height, width);
    gameLoop(maze, height, width);

    return 0;
}