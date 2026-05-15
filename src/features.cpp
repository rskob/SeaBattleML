#include "features.h"
#include <map>
#include <iostream>
#include <algorithm>


static int CountEmptyDirection(const Board& board, Point pt, int dx, int dy)
{
    int count = 0;

    for (int x = pt.x + dx, y = pt.y + dy; InBounds(x, y); x += dx, y += dy){
        if (board[y][x] == Cell::Miss) break;
        count++;
    }

    return count;
}

Features::Features(Point pt, Board& board, std::map<int, int> shipCount)
{
    setHits(pt, board);
    setPlacements(pt, board);
    setShips(shipCount);
}

void Features::setHits(Point pt, Board& board)
{
    if (InBounds(pt.x - 1, pt.y) and board[pt.y][pt.x - 1] == Cell::Hit) hitLeft++;
    if (InBounds(pt.x + 1, pt.y) and board[pt.y][pt.x + 1] == Cell::Hit) hitRight++;
    if (InBounds(pt.x, pt.y - 1) and board[pt.y - 1][pt.x] == Cell::Hit) hitDown++;
    if (InBounds(pt.x, pt.y + 1) and board[pt.y + 1][pt.x] == Cell::Hit) hitUp++;

    if(InBounds(pt.x - 1, pt.y + 1) and board[pt.y + 1][pt.x - 1] == Cell::Hit) hitDiagonal |= 1;
    if(InBounds(pt.x + 1, pt.y + 1) and board[pt.y + 1][pt.x + 1] == Cell::Hit) hitDiagonal |= 1;
    if(InBounds(pt.x - 1, pt.y - 1) and board[pt.y - 1][pt.x - 1] == Cell::Hit) hitDiagonal |= 1;
    if(InBounds(pt.x + 1, pt.y - 1) and board[pt.y - 1][pt.x + 1] == Cell::Hit) hitDiagonal |= 1;

    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            if(!InBounds(pt.x + x, pt.y + y) or (x == 0 and y == 0)) continue;
            nearMissCount += board[pt.y + y][pt.x + x] == Cell::Miss;
        }
    }
    coordSumMod2 = (pt.x + pt.y) % 2;
}

void Features::setPlacements(Point pt, Board& board)
{
    int horizontalLeft = CountEmptyDirection(board, pt, -1, 0);
    int horizontalRight = CountEmptyDirection(board, pt, 1, 0);
    int verticalDown = CountEmptyDirection(board, pt, 0, -1);
    int verticalUp = CountEmptyDirection(board, pt, 0, 1);


    nearestBoardEdgeDistance = std::min({pt.x, pt.y, BOARD_SIZE - 1 - pt.x, BOARD_SIZE - 1 - pt.y});
    placements2 = ((std::min(horizontalLeft, 1) + std::min(horizontalRight, 1) + 1) - 2 + 1) + 
                  ((std::min(verticalDown, 1) + std::min(verticalUp, 1) + 1) - 2 + 1);
    if(placements2 <= 0){
        placements2 = 0;
        return;
    };
    placements3 = ((std::min(horizontalLeft, 2) + std::min(horizontalRight, 2) + 1) - 3 + 1) + 
                  ((std::min(verticalDown, 2) + std::min(verticalUp, 2) + 1) - 3 + 1);
    if(placements3 <= 0){
        placements3 = 0;
        return;
    };
    placements4 = ((std::min(horizontalLeft, 3) + std::min(horizontalRight, 3) + 1) - 4 + 1) + 
                  ((std::min(verticalDown, 3) + std::min(verticalUp, 3) + 1) - 4 + 1);
    if(placements4 < 0){
        placements4 = 0;
    }
}

void Features::setShips(std::map<int, int> shipCount)
{
    shipsLeft4 = shipCount[4];
    shipsLeft3 = shipCount[3];
    shipsLeft2 = shipCount[2];
    shipsLeft1 = shipCount[1];
}

std::string Features::ToString()
{
    return std::to_string(bias) + ";" +
           std::to_string(hitLeft) + ";" +
           std::to_string(hitRight) + ";" +
           std::to_string(hitUp) + ";" +
           std::to_string(hitDown) + ";" +
           std::to_string(hitDiagonal) + ";" +
           std::to_string(nearMissCount) + ";" + 
           std::to_string(placements2 * 10) + ";" +
           std::to_string(placements3 * 10) + ";" +
           std::to_string(placements4 * 10) + ";" +
           std::to_string(shipsLeft4) + ";" +
           std::to_string(shipsLeft3) + ";" + 
           std::to_string(shipsLeft2) + ";" +
           std::to_string(coordSumMod2);
}

void SaveFeature(std::ostream& os, Features& f, int target)
{
    os << f.ToString() << ";" << target << std::endl;
}

Features GetFeatures(Point pt, Board& board, std::map<int, int> shipCount)
{
    return Features(pt, board, shipCount);
}
