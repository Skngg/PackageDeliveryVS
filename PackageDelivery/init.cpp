#include "init.h"

void initFromFile(std::vector<Point>& _points)
{
	std::ifstream file("../DATABASE.txt");
	std::string bufferStrLine;
	std::string bufferStr;
	std::vector<int> bufferInt;
	int pointID = 0;

	while (std::getline(file, bufferStrLine))
	{
		if (bufferStrLine.compare("POINT") == 0) // CHECK FOR OPENING IDENTIFIER
		{
			// ASSUMING THAT POINTS ARE SAVED IN INCREASING ORDER VIA THEIR ID
			Point tmpPoint = Point(pointID);
			pointID++;

			while (std::getline(file, bufferStrLine))
			{
				if (bufferStrLine.compare("POINT") == 0) { break; } // CHECK FOR CLOSING IDENTIFIER <=> CHECK IF POINT HAS NO MORE PACKAGES

				bufferInt.clear();
				std::istringstream bufferStream(bufferStrLine);
				while (std::getline(bufferStream, bufferStr, ','))
				{
					bufferInt.push_back(std::stoi(bufferStr));
				}
				Package tmpPackage = Package(bufferInt.at(0), bufferInt.at(1));
				tmpPoint.addPackage(tmpPackage);
			}

			_points.push_back(tmpPoint);
		}
	}
}


std::vector<std::vector<int>> costMatrixReadCSV()
{
	std::ifstream file("../data1.csv");
	std::vector<std::vector<int>> matrix;

	std::string bufferStrLine;
	std::string bufferStr;
	std::vector<int> bufferInt;


	while (std::getline(file, bufferStrLine))
	{
		bufferInt.clear();
		std::istringstream bufferStream(bufferStrLine);
		while (std::getline(bufferStream, bufferStr, ','))
		{
			bufferInt.push_back(std::stoi(bufferStr));
		}
		matrix.push_back(bufferInt);
	}

	return matrix;
}
