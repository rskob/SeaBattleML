#pragma once
#include "types.h"
#include <random>

void ResetGame(GameState&, std::mt19937&);
void HandlePlayerTurn(GameState& g, std::mt19937& r, bool);
void HandleEnemyTurn(GameState&, std::vector<double>& weights, bool IgnoreDelay);
bool MouseToCell(int,int,int,int,Point&);
