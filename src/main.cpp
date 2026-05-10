#include "game.h"
#include "render.h"
#include "raylib.h"
#include <random>
#include "bot.h"
#include "learn.h"
#include <string.h>
#include <iostream>
#include <string.h>



#define LEARN 0
#define DEFAULT Adam
#define DEFAULT_GAMES 100


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
                if(algName == "Adam"){
                    Adam();
                    saveLastAlgInfo(algName);
                } else if (algName == "SGD"){
                    SGD();
                    saveLastAlgInfo(algName);    
                } else{
                    std::cout << "Unknown algorithm: " << "'" << algName << "'" << std::endl;
                }
                return 0;
            }
            std::cout << "Using default algorithm" << std::endl;
            Adam();
            return 0;
        } else if (std::string(argv[1]) == "--random")
        {
            random = true;
            if(argc > 2 && std::string(argv[2]) == "--hide")
            {
                SetConfigFlags(FLAG_WINDOW_HIDDEN);
            }
        } else if (std::string(argv[1]) == "--accuracy"){
            int gameNumber = DEFAULT_GAMES;
            if(argc > 2){
                gameNumber = std::stoi(argv[2]);
            } 
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
