#include "Game.h"

int main()
{
	Snake& snake = Snake::getInstance();
	snake.init();
	snake.run();
}
