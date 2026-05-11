#include "game.h"
#include "render.h"
#include "raylib.h"
#include <random>
#include "bot.h"
#include "learn.h"
#include <string.h>
#include <iostream>
#include <string.h>
#include "options.h"



int loop(GameState& g, std::mt19937& rng, std::vector<double> weights, bool random)
{
    ResetGame(g, rng);
    while(!g.gameOver){
        if(g.turn==Turn::Player) HandlePlayerTurn(g, random);
        else HandleEnemyTurn(g,rng, weights, true);
    }
    return getMove();
}


int main(int argc, char* argv[]){
    std::mt19937 rng(std::random_device{}());
    std::vector<double> weights;
    readWeights(weights);
    Config config;
    parseArguments(argc, argv, config);

    if(!config.errorMessage.empty()){
        std::cerr << "Error: " << config.errorMessage << std::endl;
        return 1;
    }
    
    switch (config.mode)
    {
        case Mode::Learn:
        {
            std::string algName = config.algName;
            algoritmPointer alg = getAlgorithm(algName);
            if(alg == nullptr){
                std::cerr << "Error: Unknown algorithm name: " << algName << std::endl;
                return 1;
            }
            alg();
            saveLastAlgInfo(algName);
            return 0;
        }
        case Mode::Accuracy:
        {
            double mean = 0.0;
            for(int i = 1; i < config.gameNumber + 1; i++){
                GameState g;
                int moves = loop(g, rng, weights, true);
                if(!g.playerWon) mean += moves;
                else {
                    i--;
                    continue;
                }
                std::cout << "Game " << i << "/" << config.gameNumber << "\r";
                std::cout.flush();
            }
            clearConsole();
            std::string algName;
            getLastAlgInfo(algName);
            std::cout << "Algorithm: " << algName << std::endl;
            std::cout << "Average move number for " << config.gameNumber << " games: " << mean / config.gameNumber << std::endl;
            return 0;
        }
        case Mode::CollectData:
        {
            setMemorize(true);
            for(int i = 1; i <= config.gameNumber; i++){
                std::cout << "Game " << i << "/" << config.gameNumber << "\r";
                std::cout.flush();
                GameState g;
                loop(g, rng, weights, true);
            }
            clearConsole();
            std::cout << "Collected data from " << config.gameNumber << " games" << std::endl;
            return 0;
        }
        case Mode::Play:
        {
            if(config.hide) SetConfigFlags(FLAG_WINDOW_HIDDEN);

            InitWindow(1280,560,"SeaBattle");
            SetTargetFPS(60);

            GameState g;
            ResetGame(g,rng);

            while(!WindowShouldClose()){
                if(!g.gameOver){
                    if(g.turn==Turn::Player) HandlePlayerTurn(g, config.random);
                    else HandleEnemyTurn(g,rng, weights, false);
                }
                else if(IsKeyPressed(KEY_R)){
                    ResetGame(g,rng);
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                if(!g.gameOver)
                    DrawText(g.turn == Turn::Player ? "PLAYER TURN" : "BOT TURN", 480, 20, 40, BLACK);      
                DrawUI(g);
                EndDrawing();
            }

            CloseWindow();
        }
    }
}
