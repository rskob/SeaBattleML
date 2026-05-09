#include "game.h"
#include "render.h"
#include "raylib.h"
#include <random>
#include "bot.h"
#include "learn.h"
#include <string.h>
#include <iostream>
#include <string.h>
#include <thread>
#include <chrono>


#define LEARN 0
#define DEFAULT Adam

int main(int argc, char* argv[]){
    bool random = false;
    if(argc > 1){
        if(std::string(argv[1]) == "--learn"){
            if(argc > 2){
                if(std::string(argv[2]) == "Adam"){
                    Adam();
                    return 0;
                } else if (std::string(argv[2]) == "SGD"){
                    SGD();
                    return 0;
                } else{
                    std::cout << "Unknown algorithm: " << "'" << std::string(argv[2]) << "'" << std::endl;
                }
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
        }
        else std::cout << "Unknown argument: " << argv[1] << std::endl;
    }

    std::mt19937 rng(std::random_device{}());
    InitWindow(1280,560,"SeaBattle");
    SetTargetFPS(60);

    GameState g;
    ResetGame(g,rng);

    while(!WindowShouldClose()){
        if(!g.gameOver){
            if(g.turn==Turn::Player) HandlePlayerTurn(g, random);
            else HandleEnemyTurn(g,rng);
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
