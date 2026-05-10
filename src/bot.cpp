#include "bot.h"
#include "board.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "features.h"
#include <map>
#include <fstream>
#include <iostream>
#include "learn.h"
#include <algorithm>

#define MIN_ITERATIONS 0
static int counter = 0;
static bool memorize = false;


static std::vector<Point> availablePoints;
static std::vector<Point> currentHits;



int getMove()
{
    return counter;
}

void setMemorize(bool value)
{
    memorize = value;
}

std::map<int, int> shipCount = {{1, 4}, {2, 3}, {3, 2}, {4, 1}};



void ResetBot(GameState& g)
{
    counter = 0;
    availablePoints.clear();
    currentHits.clear();

    shipCount = {{1, 4}, {2, 3}, {3, 2}, {4, 1}};

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            availablePoints.push_back({x, y});
        }
    }
}


bool PointsRelated(Point a, Point b)
{
    return
        (a.x + 1 == b.x && a.y == b.y) ||
        (a.x - 1 == b.x && a.y == b.y) ||
        (a.x == b.x && a.y + 1 == b.y) ||
        (a.x == b.x && a.y - 1 == b.y);
}

bool SamePoint(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

void RemovePointFromVector(std::vector<Point>& v, Point p)
{
    for (int i = 0; i < v.size(); i++)
    {
        if(SamePoint(v[i], p))
        {
            v[i] = v.back();
            v.pop_back();
            return;
        }
    }
}

// Point GetBotShot() {
//     srand(time(0));
//     return Point{rand() % BOARD_SIZE, rand() % BOARD_SIZE};
// }

void readWeights(std::vector<double>& weights)
{
    std::ifstream weightsInfo("../data/weights.txt");
    double w;
    while(weightsInfo >> w){
        weights.push_back(w);
    }
}

Point GetBotShot(Board& playerBoard, std::vector<double> &weights)
{
    std::ifstream weightsInfo("weights.txt");
    std::vector<std::pair<Point, double>> scores;
    for(Point pt: availablePoints){
        Features ft(pt, playerBoard, shipCount);
        std::vector<double> featuresLine = split(ft.ToString(), ';');
        double result = model(weights, featuresLine);
        scores.push_back({pt, result});
    }
    std::sort(scores.begin(), scores.end(),
    [](const std::pair<Point, double>& a, const std::pair<Point, double>& b) {
        return a.second > b.second;
    });
    return scores[0].first;
}

void SetBotResult(Result result, Point p, Board& playerBoard)
{
    counter++;
    RemovePointFromVector(availablePoints, p);

    if(result == Result::Hit)
    {
        currentHits.push_back(p);
        return;
    }

    std::ofstream file("../data/data.txt", std::ios::app);
    if(counter > MIN_ITERATIONS){
        for(Point pt: availablePoints){
            Features f = GetFeatures(pt, playerBoard, shipCount);
            int target = -1;
            if(playerBoard[pt.y][pt.x] == Cell::Ship) target = 1;
            int number = rand();
            if(memorize){
                if(target == -1 and (number % 2 == 0 or number % 3 == 0 or number % 4 == 0)) continue;
                SaveFeature(file, f, target);
            }
            
        }
        file.close();
    }

    if(result == Result::Sink)
    {
        currentHits.push_back(p);

        std::vector<Point> currentShip;
        currentShip.push_back(p);

        bool changed = true;

        while(changed)
        {
            changed = false;
            for (Point hit : currentHits)
            {
                bool alreadyInShip = false;

                for (Point shipPoint : currentShip)
                {
                    if (SamePoint(hit, shipPoint))
                    {
                        alreadyInShip = true;
                        break;
                    }
                }

                if (alreadyInShip)
                    continue;

                for (Point shipPoint : currentShip)
                {
                    if (PointsRelated(hit, shipPoint))
                    {
                        currentShip.push_back(hit);
                        changed = true;
                        break;
                    }
                }
            }
        }

        shipCount[currentShip.size()]--;

        for (Point point : currentShip)
        {
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    Point forDeletion{point.x + x, point.y + y};

                    if (InBounds(forDeletion.x, forDeletion.y))
                    {
                        RemovePointFromVector(availablePoints, forDeletion);
                    }
                }
            }
            RemovePointFromVector(currentHits, point);
        }
    }
}

