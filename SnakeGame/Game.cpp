#include "Game.h"

#include <iostream>
#include <stdlib.h>
#include <random>
#include <conio.h>
#include <time.h>
#include <Windows.h>
using namespace std;

Snake* Snake::instance = nullptr;
Snake& Snake::getInstance()
{
	if (!instance)
	{
		instance = new Snake();
		if (!instance) throw std::runtime_error("Failed to create instance");
	}
	return *instance;
}

int score = 0; 
int len = score + 1;
DIR dir = DIR::R;
PROGRESS pro = PROGRESS::PLAYING;

void Snake::swap(Cell** A, Cell** B)
{
	Cell* tmp = *A;
	*A = *B;
	*B = tmp;

	int tmpX = (*A)->X;
	(*A)->X = (*B)->X;
	(*B)->X = tmpX;

	int tmpY = (*A)->Y;
	(*A)->Y = (*B)->Y;
	(*B)->Y = tmpY;
}

void Snake::createApple()
{
	srand(time(NULL));

	int rX, rY;
	do
	{
		rX = rand() % W + 1;
		rY = rand() % H + 1;
	} while (cell[rX][rY]->state != STATE::EMPTY);

	cell[rX][rY]->state = STATE::APPLE;
}

void Snake::input()
{
	char inputDir;
	bool correctInput;
	int Hx = head->X, Hy = head->Y;

	if (_kbhit())
	{
		do
		{
			inputDir = _getch();

			if (inputDir == 27)
				exit(0);

			correctInput = inputDir == 'W' || inputDir == 'A' || inputDir == 'S' || inputDir == 'D'
				|| inputDir == 'w' || inputDir == 'a' || inputDir == 's' || inputDir == 'd';

			if (!correctInput)
				printf("Wrong input\n");

		} while (!correctInput);

		if (inputDir == 'W' || inputDir == 'w') dir = DIR::U;
		else if (inputDir == 'S' || inputDir == 's') dir = DIR::D;
		else if (inputDir == 'A' || inputDir == 'a') dir = DIR::L;
		else if (inputDir == 'D' || inputDir == 'd') dir = DIR::R;
	}
}

void Snake::update()
{
	if (score == W * H - 1)
	{
		pro = PROGRESS::WIN;
		return;
	}

	int Hx = head->X, Hy = head->Y;
	int Nx = Hx, Ny = Hy;

	if (dir == DIR::U) Nx -= 1;
	else if (dir == DIR::D) Nx += 1;
	else if (dir == DIR::L) Ny -= 1;
	else if (dir == DIR::R) Ny += 1;

	if (cell[Nx][Ny]->state == STATE::APPLE)
	{
		score++;

		cell[Nx][Ny]->state = STATE::BODY;
		body.push_back(cell[Nx][Ny]);
		swap(&cell[Hx][Hy], &cell[Nx][Ny]);
		createApple();
	}
	else
	{
		if (cell[Nx][Ny]->state == STATE::EMPTY)
			swap(&cell[Hx][Hy], &cell[Nx][Ny]);
		else if (cell[Nx][Ny]->state == STATE::WALL || cell[Nx][Ny]->state == STATE::BODY)
			pro = PROGRESS::DEFEAT;

		int curX = Hx, curY = Hy;
		int saveX, saveY;
		for (int b = body.size() - 1; b >= 0; b--)
		{
			saveX = body[b]->X;
			saveY = body[b]->Y;

			swap(&cell[saveX][saveY], &cell[curX][curY]);

			curX = saveX;
			curY = saveY;
		}
	}

	return;
}

void Snake::printGrid()
{
	system("CLS");
	for (int i = 0; i < W + 2; i++)
	{
		for (int j = 0; j < H + 2; j++)
		{
			STATE here = cell[i][j]->state;

			if (here == STATE::EMPTY)
				printf("  ");
			if (here == STATE::BODY)
				printf("¡Ü");
			if (here == STATE::APPLE)
				printf("¢À");
			if (here == STATE::WALL)
				printf("¡á");
			if (here == STATE::HEAD)
			{
				if (dir == DIR::U) printf("¡ã");
				if (dir == DIR::D) printf("¡å");
				if (dir == DIR::L) printf("¢¸");
				if (dir == DIR::R) printf("¢º");
			}
		}

		printf("\n");
	}

	printf("\nScore: %d\n", score);
}

void Snake::init()
{
	for (int i = 0; i < W + 2; i++)
		for (int j = 0; j < H + 2; j++)
		{
			if (i == 0 || i == W + 1 || j == 0 || j == H + 1)
				cell[i][j] = new Cell(STATE::WALL, i, j);
			else
				cell[i][j] = new Cell(STATE::EMPTY, i, j);
		}

	cell[1 + W / 2][1 + H / 2]->state = STATE::HEAD;
	head = cell[1 + W / 2][1 + H / 2];
	createApple();
}

void finish()
{
	if (pro == PROGRESS::DEFEAT)
		printf("Game Over..\n");
	else
		printf("You Won!\n");
}

void Snake::run()
{
	while (pro == PROGRESS::PLAYING)
	{
		printGrid();
		Sleep(150);
		input();
		update();
	}
	finish();
}