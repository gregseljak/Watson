# pragma once
// CSV manipulator

# include <cmath>
# include <fstream>
# include <vector>

int writecsv(std::string name, const std::vector<std::vector<double>>& data);
std::vector <std::vector<double>> readcsv(std::string filename);
int testcsv();