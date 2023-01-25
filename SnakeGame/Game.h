#pragma once
#include <vector>
using namespace std;

static constexpr int W = 10;
static constexpr int H = 10;

enum class DIR { L, R, U, D };

enum class STATE
{
	EMPTY,
	HEAD,
	BODY,
	APPLE,
	WALL
};

enum class PROGRESS
{
	PLAYING,
	DEFEAT,
	WIN
};

class Cell
{
public:
	Cell() { state = STATE::EMPTY; };
	Cell(STATE s, int x, int y) : state(s), X(x), Y(y) { };
	virtual ~Cell() {};

	int X = -1, Y = -1;
	STATE state;
};

class Snake
{
protected:
	static Snake* instance;

public:
	static Snake& getInstance();
	Snake() { init(); };
	virtual ~Snake() {};

	void init();
	void run();

	void swap(Cell** A, Cell** B);
	void createApple();
	void input();
	void update();
	bool check();
	void printGrid();
	

	Cell* cell[W + 2][H + 2] = { nullptr };
	Cell* head = nullptr;
	vector<Cell*> body;
};