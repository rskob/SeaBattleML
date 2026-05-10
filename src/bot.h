#pragma once
#include "types.h"
#include <random>


int getMove();
Point GetBotShot(Board& playerBoard, std::vector<double>& weights); // Функция которая отдает координаты выстрела бота
void ResetBot(GameState& g);
void readWeights(std::vector<double>& weights);
void SetBotResult(Result resulst, Point p, Board& playerBoard); // Функция, которая говорит боту результат выстрела
void setMemorize(bool value);

