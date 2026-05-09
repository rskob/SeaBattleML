#pragma once
#include "types.h"
#include <random>

struct Bot{

};

int getMove();
Point GetBotShot(Board& playerBoard); // Функция которая отдает координаты выстрела бота
//void RemoveCell(Point p);
void ResetBot(GameState& g);
void SetBotResult(Result resulst, Point p, Board& playerBoard); // Функция, которая говорит боту результат выстрела