# include "csv_manip.h"
# include <iostream>
# include <vector>
# include <cctype>
# include <string>
# include <cmath>
# include <fftw3.h>
# include <chrono>
# include <fstream>
# pragma warning(disable:4996)



int testcsv() {
	std::vector<std::vector<double>> myfirstexport;
	for (int i{ 0 }; i < 10; i++) {
		myfirstexport.push_back(std::vector<double>{0});
		std::cout << "\n i : " << i;
		for (int j{ 0 }; j < 3; j++) {
			(myfirstexport[i]).push_back(j);
			std::cout << " j : " << j;
		};
	};
	writecsv("myfirstexport", myfirstexport);
	return 0;
	
}

int writecsv(std::string name, const std::vector<std::vector<double>> &data) {
	std::ofstream myfile;
	int nl{ static_cast<int>(name.length()) };
	if (name.at(nl - 1) != *"v" || name.at(nl - 2) != *"s" || name.at(nl - 3) != *"c" || name.at(nl - 4) != *".") {
		name += ".csv";
	}

	std::cout << "\n reading data0.size as " << data[0].size() << " and data.size() as " << data.size();
	myfile.open(name);
	for (unsigned int i{ 0 }; i < data.size(); i++) {
		for (unsigned int j{ 0 }; j < (data[i]).size(); j++) {
			myfile << data[i][j] << ",";
		};
		
		myfile << "\n";
	}
	myfile.close();
	return 0;
}

std::vector <std::vector<double>> readcsv(std::string filename) {
	FILE* pFile;
	long lSize{};
	char* buffer{ NULL };
	size_t result;
	int nl{ static_cast<int>(filename.length()) };
	if (filename.at(nl - 1) != *"v" || filename.at(nl - 2) != *"s" || filename.at(nl - 3) != *"c" || filename.at(nl - 4) != *".") {
		filename += ".csv";
	}
	pFile = fopen(filename.c_str(), "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(1); }
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);
	buffer = new char[sizeof(char) * lSize];
	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); std::cout << result << " " << lSize; exit(3); }
	fclose(pFile);


	std::vector <std::vector<double>> csv_grid{ std::vector<double>{} };
	csv_grid.at(0).push_back(0);
	int row{ 0 };
	int col{ 0 };
	char ele{};
	std::string cell{};
	for (int i{ 0 }; i < lSize; i++) {
		ele = *(buffer + i);
		
		if (ele == *"," || ele == *"\n") {

			csv_grid.at(col).at(row) = atof(cell.c_str());
			//std::cout << "," << csv_grid.at(col).at(row); //troubleshooting line
			cell = "";
			(csv_grid.at(col)).push_back({});
			row++;

			if (ele == *"\n") {
				csv_grid.push_back(std::vector<double>{});
				col++;
				row = 0;
				std::cout << "/n" << "\n"; //troubleshootingline
				csv_grid.at(col).push_back({});
			}
		}
		else {
			cell += ele;
		}
	}
	
	csv_grid.pop_back(); // gets rid of trailing 0 row
	return csv_grid;
}
