#include "init.h"

void initFromFile(std::vector<Point>& _points)
{
	std::ifstream file("../DATABASE.txt");
	/*
		DATABSE FILE FORMAT BELOW:

			--BEGINNIG OF FILE--
		POINT		--OPENING IDENTIFIER--
		SRC,DST		--PACKAGE DETAILS (SOURCE POINT ID, DESTINATION POINT ID)--
		(...)
		POINT		--CLOSING IDENTIFIER--
		(..)
		POINT		--OPENING IDENTIFIER--
		(...)
		POINT		--CLOSING IDENTIFIER--
			--END OF FILE--

	*/
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
	std::ifstream file("../DATA.csv");
	/*
		COST MATRIX FILE FORMAT BELOW:
		//NO WHITECHARS BETWEEN VALUES
		// * - BIG VALUE
			--BEGINNIG OF FILE--
					  0		1	2	(...)	LASTPOINTID
		0			| BV*,	1,	3,			8
		1			| 1,	BV,	4,			6
		2			| 3,	4,	BV,			3
		(...)
		LASTPOINTID	| 8,	6,	3,			BV
			--END OF FILE--

	*/


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
