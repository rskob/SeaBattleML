#include <string>

enum class Mode{
    Play,
    Train,
    Accuracy,
    CollectData,
    Help,
    Current
};

struct Config {
    Mode mode = Mode::Play;
    std::string algName;
    bool random = false;
    bool hide = false;
    bool collectData = false;
    int gameNumber;
    std::string errorMessage;
};

void checkIfStringIsPositiveInt(const std::string& s, std::string& errorMessage);
void parseArguments(int argc, char* argv[], Config& config);
void clearConsole();
void showHelp();
