#include <fstream>
#include <vector>
#include <numeric>
#include <cmath>
#include "learn.h"
#include <iostream>
#include <random>
#include <algorithm>


double model(const std::vector<double>& weights, const std::vector<double>& x)
{
    return std::inner_product(weights.begin(), weights.end(), x.begin(), 0.0);
}

double loss(const std::vector<double>& weights, const std::vector<double>& x, int y)
{
    double Margin = model(weights, x) * y;
    return std::exp(-Margin);
}



std::vector<double> gradient(const std::vector<double>& weights, const std::vector<double>& x, int y)
{
    int size = weights.size();
    std::vector<double> grad(size);
    double loss_value = -y * model(weights, x);
    for(int i = 0; i < size; i++){
        grad[i] = -y * x[i] * std::exp(loss_value);
    }
    return grad;
}

std::vector<double> split(const std::string& line, char delimiter)
{
    std::vector<double> result;
    std::string current;
    for(char c: line){
        if(c == delimiter){
            result.push_back(std::stod(current));
            current.clear();
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        result.push_back(std::stod(current));
    }
    return result;
}

void readFeatures(std::vector<std::vector<double>>& features, std::vector<int>& targets)
{
    std::ifstream data("../data/data.txt");
    std::string line;
    while(std::getline(data, line)){
        std::vector<double> dataLine = split(line, ';');
        std::vector<double> pointFeatures(dataLine.begin(), dataLine.end() - 1);
        features.push_back(pointFeatures);
        targets.push_back(dataLine.back());
    }
}

void saveWeights(const std::vector<double>& weights)
{
    std::ofstream file("../data/weights.txt");
    for(double w: weights){
        file << w << " ";
    }
    file << std::endl;
}



void Adam()
{
    std::vector<std::vector<double>> features;
    std::vector<int> targets;
    readFeatures(features, targets);
    unsigned int trainingSetSize = features.size();

    std::cout << "Adam is learning on " << trainingSetSize << " samples" << std::endl;

    std::vector<double> weights(14, 0.01);
    unsigned int iterations = trainingSetSize;
    int batchSize = 20;
    double gamma = 0.99;
    double beta = 0.999;
    double lm = 0.0001;
    double reg = 0.000005;
    double eps = 1e-8;

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, trainingSetSize - 1);
    std::vector<double> impulse(14, 0);
    std::vector<double> gradMemory(14, 0);
    for(int i = 0; i < iterations; i++){
        if(i % (iterations / 100) == 0) {
            std :: cout << "Progress: " << (i * 100) / iterations << "%" << "\r";
            std::cout.flush();
        }
        std::vector<double> gradSum(14, 0);
        for(int k = 0; k < batchSize; k++){
            int m = dist(gen);
            std::vector<double> nextGrad = gradient(weights, features[m], targets[m]);
            for(int n = 0; n < 14; n++){
                gradSum[n] += nextGrad[n] / batchSize;
            }
        }
        for(int n = 0; n < 14; n++){
            impulse[n] = gamma * impulse[n] + (1 - gamma) * (gradSum[n]);
            gradMemory[n] = beta * gradMemory[n] + (1 - beta) * (gradSum[n] * gradSum[n]);

            double impulseCorrection = impulse[n] / (1 - std::pow(gamma, i + 1));
            double gradMemoryCorrection = gradMemory[n] / (1 - std::pow(beta, i + 1));
            double currentReg = (n >= 6) ? 0.0: reg;
            weights[n] = weights[n] * (1 - currentReg * lm) - lm * impulseCorrection / (std::sqrt(gradMemoryCorrection) + eps);
        }
        weights[5] = std::max(weights[5], -3.0);
    }

    std::cout << "Training completed. Final weights: " << std::endl;
    for(int i = 0; i < 14; i++){
        std::cout << weights[i] << " ";
    }
    std::cout << std::endl;

    saveWeights(weights);
}

void SGD()
{
    std::vector<std::vector<double>> features;
    std::vector<int> targets;
    readFeatures(features, targets);
    unsigned int trainingSetSize = features.size();

    std::cout << "SGD is learning on " << trainingSetSize << " samples" << std::endl;

    std::vector<double> weights(14, 0);
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, trainingSetSize - 1);
    std::vector<double> learningRate(14, 0.005);
    int batchSize = 16;
    int iterations = 500000;
    double lm = 0.0001;
    for(int i = 0; i < trainingSetSize; i++){
        features[i][7] /= 20.0;
        features[i][8] /= 20.0;
        features[i][9] /= 20.0;
    }


    for(int i = 0; i < iterations; i++){
        if(i % (iterations / 100) == 0) {
            std :: cout << "Progress: " << (i * 100) / iterations << "%" << "\r";
            std::cout.flush();
        }
        std::vector<double> batchGrad(14, 0);
        for(int j = 0; j < batchSize; j++){
            int k = dist(gen);
            std::vector<double> nextGrad = gradient(weights, features[k], targets[k]);
            for(int m = 0; m < 14; m++){
                batchGrad[m] += nextGrad[m] / batchSize;
            }
        }
        for(int f = 0; f < 14; f++){
            weights[f] = weights[f] * (1 - lm * learningRate[f]) - learningRate[f] * batchGrad[f];
        }
    }
    std::cout << "Training completed. Final weights: " << std::endl;
    for(int i = 0; i < 14; i++){
        std::cout << weights[i] << " ";
    }
    saveWeights(weights);
}
