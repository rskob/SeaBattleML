#include <vector>

std::vector<double> split(const std::string& line, char delimiter);
double model(const std::vector<double>& weights, const std::vector<double>& x);
void Adam();
void SGD();
void saveLastAlgInfo(const std::string& algName);
void getLastAlgInfo(std::string& algName);
using algoritmPointer = void(*)();
algoritmPointer getAlgorithm(const std::string& algName);