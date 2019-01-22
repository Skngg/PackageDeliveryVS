#include "init.h"

void initFromFile(std::vector<Point>& _points, int n)
{
	std::ifstream file("../PACKAGES.txt");
	/*
		DATABSE FILE FORMAT BELOW:

			--BEGINNIG OF FILE--
		SRC,DST		--PACKAGE DETAILS (SOURCE POINT ID, DESTINATION POINT ID)--
		(...)
			--END OF FILE--

	*/
	std::string bufferStrLine;
	std::string bufferStr;
	std::vector<int> bufferInt;
	int pointID = 0;
	Point* tmpPoint = new Point(pointID);

	
	while (std::getline(file, bufferStrLine))
	{
		
		if (tmpPoint == nullptr)
		{
			tmpPoint = new Point(pointID);
		}

		// PARSE LINE INTO NUMBERS
		std::istringstream bufferStream(bufferStrLine);
		bufferInt.clear();
		while (std::getline(bufferStream, bufferStr, ','))
		{
			bufferInt.push_back(std::stoi(bufferStr));
		}

		if (bufferInt.at(0) == pointID)
		{
			
			Package tmpPackage = Package(bufferInt.at(0), bufferInt.at(1));
			tmpPoint->addPackage(tmpPackage);
		}
		else
		{
			do
			{
				_points.push_back(*tmpPoint);		// ADD EXISTING POINT TO THE _points VECTOR
				pointID++;							// GET NEXT POINT ID
				tmpPoint = new Point(pointID);		// CREATE NEW TEMPORARY POINT
			} while (pointID != bufferInt.at(0));	

			Package tmpPackage = Package(bufferInt.at(0), bufferInt.at(1));
			tmpPoint->addPackage(tmpPackage);
			
		}
	}
	_points.push_back(*tmpPoint);
	
	while (pointID != n)					// CREATE REMAINING POINTS WHEN NO PACKAGES ARE SPECIFIED
	{
		pointID++;							// GET NEXT POINT ID
		tmpPoint = new Point(pointID);		// CREATE NEW TEMPORARY POINT
		_points.push_back(*tmpPoint);		// ADD EXISTING POINT TO THE _points VECTOR
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
