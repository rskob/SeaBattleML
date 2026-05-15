#include <string>
#include <algorithm>
#include <iostream>
#include "learn.h"
#include "options.h"


void parseArguments(int argc, char* argv[], Config& config)
{
    if(argc > 1){
        std::string arg1 = std::string(argv[1]);
        if(arg1 == "--learn"){
            config.mode = Mode::Learn;
            if(argc > 2){
                config.algName = std::string(argv[2]);
            } else {
                config.errorMessage = "Missing algorithm name for --learn option";
            }
        } else if (arg1 == "--random"){
            config.random = true;
            if(argc > 2 && std::string(argv[2]) == "--hide"){
                config.hide = true;
            }
        } else if(arg1 == "--accuracy"){
            config.mode = Mode::Accuracy;
            if(argc > 2){
                checkIfStringIsPositiveInt(std::string(argv[2]), config.errorMessage);
                if(config.errorMessage.empty()){
                    config.gameNumber = std::stoul(argv[2]);
                }
            } else {
                config.errorMessage = "Missing game number for --accuracy option";
            }
        } else if(arg1 == "--collect-data"){
            if(argc > 2){
                config.mode = Mode::CollectData;
                checkIfStringIsPositiveInt(std::string(argv[2]), config.errorMessage);
                if(config.errorMessage.empty()){
                    config.gameNumber = std::stoul(argv[2]);
                }
            } else {
                config.errorMessage = "Missing game number for --collect-data option";
            }  
        } else if(arg1 == "--help"){
            config.mode = Mode::Help;
        }
        else {
            config.errorMessage = "Unknown argument: " + arg1;
        }
    }
}

void checkIfStringIsPositiveInt(const std::string& s, std::string& errorMessage)
{
    if(!s.empty() && std::all_of(s.begin(), s.end(), ::isdigit)){
        try {
            int num = std::stoi(s);
            if(num < 0) errorMessage = s + ": thissss argument must be a positive integer";
        } 
        catch (const std::out_of_range& e) {
            errorMessage = s + ": Too large to be true";
        }
        if (s == "0") errorMessage = s + ": this argument must be a positive integer";
        
    } else {
        errorMessage = s + ": this argument must be a positive integer";
    }
}

void clearConsole()
{
    std::cout << "\r\33[2K";
}

void showHelp()
{
    std::cout << "===========================================================" << std::endl;
    std::cout << "                 SEABATTLE: PLAY WITH AI               " << std::endl;
    std::cout << "===========================================================" << std::endl;
    
    std::cout << "\nUSAGE:" << std::endl;
    std::cout << "  ./SeaBattle [OPTIONS]" << std::endl;

    std::cout << "\nGAME MODES:" << std::endl;
    std::cout << "  (no args)             Manual Play: Player vs Bot" << std::endl;
    std::cout << "  --random              Simulation: Random moves vs Bot" << std::endl;

    std::cout << "\nDATA & ANALYSIS:" << std::endl;
    std::cout << "  --collect-data <n>    Play <n> games and save results to dataset" << std::endl;
    std::cout << "  --accuracy <n>        Evaluate bot performance over <n> games" << std::endl;

    std::cout << "\nMACHINE LEARNING:" << std::endl;
    std::cout << "  --learn <algorithm>   Train the linear classifier" << std::endl;
    
    std::cout << "\n  Available Algorithms:" << std::endl;
    std::cout << "    [1] Adam  - Adaptive Moment Estimation (Best)" << std::endl;
    std::cout << "    [2] SGD   - Stochastic Gradient Descent (Classic)" << std::endl;
    std::cout << "    [3] SAG   - Stochastic Average Gradient (Stable)" << std::endl;

    std::cout << "\n===========================================================" << std::endl;
    std::cout << "  Hint: Use --hide with --random for game in headlles mode" << std::endl;
    std::cout << "===========================================================" << std::endl;
}

