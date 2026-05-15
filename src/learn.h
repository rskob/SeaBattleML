#include <vector>
#include <string>
#include <cstdint> // Для uint64_t

using algoritmPointer = void(*)(std::vector<std::vector<double>>&, std::vector<int>&, uint64_t);
void Adam(std::vector<std::vector<double>>& features, std::vector<int>& targets, uint64_t trainingSetSize);
void SGD(std::vector<std::vector<double>>& features, std::vector<int>& targets, uint64_t trainingSetSize);
void SAG(std::vector<std::vector<double>>& features, std::vector<int>& targets, uint64_t trainingSetSize);
std::vector<double> split(const std::string& line, char delimiter);
double model(const std::vector<double>& weights, const std::vector<double>& x);
void saveLastAlgInfo(const std::string& algName);
void getLastAlgInfo(std::string& algName);
void readFeatures(std::vector<std::vector<double>>&, std::vector<int>&);
algoritmPointer getAlgorithm(const std::string& algName);
