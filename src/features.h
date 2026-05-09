#pragma once

#include "types.h"
#include "board.h"
#include <map>
#include <string>

struct Features {
public:
    int bias = 1;
    int hitUp = 0;
    int hitDown = 0;
    int hitLeft = 0;
    int hitRight = 0;
    int hitDiagonal = 0;

    int placements2 = 0;
    int placements3 = 0;
    int placements4 = 0;

    int nearMissCount = 0;

    int shipsLeft4 = 0;
    int shipsLeft3 = 0;
    int shipsLeft2 = 0;
    int shipsLeft1 = 0;

    int coordSumMod2 = 0;
    int nearestBoardEdgeDistance = 0;

    Features(Point pt, Board& board, std::map<int, int> shipCount);
    std::string ToString();
private:
    void setHits(Point pt, Board& board);
    void interpretateCount(int count);
    void setPlacements(Point pt, Board& board);
    void setShips(std::map<int, int> shipCount);
};

Features GetFeatures(Point pt, Board& board, std::map<int, int> shipCount);
void SaveFeature(std::ostream& os, Features& f, int target);
