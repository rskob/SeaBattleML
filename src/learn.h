std::vector<double> split(const std::string& line, char delimiter);
double model(const std::vector<double>& weights, const std::vector<double>& x);
void Adam();
void SGD();