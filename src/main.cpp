#include "game.h"
#include "render.h"
#include "raylib.h"
#include <random>
#include "bot.h"
#include "learn.h"
#include <string.h>
#include <iostream>
#include <string.h>
#include "tools.h"




int loop(std::mt19937& rng, std::vector<double> weights, bool random)
{
    GameState g;
    ResetGame(g, rng);
    while(!g.gameOver){
        if(g.turn==Turn::Player) HandlePlayerTurn(g, random);
        else HandleEnemyTurn(g,rng, weights, true);
    }
    return getMove();
}


int main(int argc, char* argv[]){
    bool random = false;
    std::mt19937 rng(std::random_device{}());
    std::vector<double> weights;
    readWeights(weights);
    if(argc > 1){
        if(std::string(argv[1]) == "--learn"){
            if(argc > 2){
                std::string algName = std::string(argv[2]);
                algoritmPointer alg = getAlgorithm(std::string(argv[2]));
                if(alg != nullptr){ 
                    alg();
                    saveLastAlgInfo(algName);
                    return 0;
                }
                std::cout << "Unknown algorithm: " << "'" << algName << "'" << std::endl;
                return 1;
            }
            std::cout << "Too few arguments" << std::endl;
            return 1;
        } else if (std::string(argv[1]) == "--random")
        {
            random = true;
            if(argc > 2 && std::string(argv[2]) == "--hide")
            {
                SetConfigFlags(FLAG_WINDOW_HIDDEN);
            }
        } else if (std::string(argv[1]) == "--accuracy"){
            if(argc > 2){
                std::string errorMessage;
                checkIfStringIsPositiveInt(std::string(argv[2]), errorMessage);
                if(!errorMessage.empty()){
                    std::cout << errorMessage << std::endl;
                    return 1;
                }
            } else {
                std::cout << "Too few arguments" << std::endl;
                return 1;
            }
            unsigned long gameNumber = std::stoul(argv[2]);
            double mean = 0.0;
            for(int i = 1; i <= gameNumber; i++){
                std::cout << "Game: " << i << "/" << gameNumber << "\r";
                std::cout.flush();
                mean += loop(rng, weights, true);
            }
            std::cout << "\r\33[2K";
            std::string algName;
            getLastAlgInfo(algName);
            std::cout << "Algorithm: " << algName << std::endl;
            std::cout << "Average moves for " << gameNumber << " games: " << mean / gameNumber << std::endl;
            return 0;
        } else if (std::string(argv[1]) == "--collect-data"){
            if(argc > 2){
                std::string errorMessage;
                checkIfStringIsPositiveInt(std::string(argv[2]), errorMessage);
                if(!errorMessage.empty()){
                    std::cout << errorMessage << std::endl;
                    return 1;
                }
            } else {
                std::cout << "Too few arguments" << std::endl;
                return 1;
            }
            setMemorize(true);
            unsigned long gameNumber = std::stoul(argv[2]);
            for(int i = 1; i <= gameNumber; i++){
                std::cout << "Game: " << i << "/" << gameNumber << "\r";
                std::cout.flush();
                loop(rng, weights, true);
            }
            return 0;
            
        }
        else std::cout << "Unknown argument: " << argv[1] << std::endl;
    }

    InitWindow(1280,560,"SeaBattle");
    SetTargetFPS(60);

    GameState g;
    ResetGame(g,rng);

    while(!WindowShouldClose()){
        if(!g.gameOver){
            if(g.turn==Turn::Player) HandlePlayerTurn(g, random);
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
