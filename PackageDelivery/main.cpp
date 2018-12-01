#include <iostream>
#include <vector>
#include "point.h"
#include "deliverer.h"
#include "package.h"
#include <random>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <ctime>



///////////////////////////////////////////////
double goalFunction(std::vector<std::vector<int>> costMatrix, std::vector<Point>& route);
template<typename T> T random(T min, T max);
std::vector<std::vector<int>> costMatrixReadCSV();
std::vector<Point> generateStart(int s);
//std::vector<Point> generateNeighbor(std::vector<Point>& old);
///////////////////////////////////////////////
int main()
{
	srand(time(NULL));
	//load data
	/*std::vector<std::vector<int>> costMatrix = costMatrixReadCSV();
	std::cout << "Macierz" << std::endl;
	for (int i = 0; i < costMatrix.size(); i++)
	{
		for (int j = 0; j < costMatrix[i].size(); j++)
		{
			std::cout << costMatrix[i][j] << "  ";
		}
		std::cout << std::endl;
	}
	std::cout << "Macierz_END" << std::endl;
	//step 1
	double T_o = 100.0;
	double T = T_o;
	double T_min = 20.0;
	std::vector<Point> x_a = generateStart(4);

	///////////////////////////////////////
	std::cout << "Startowe dane" << std::endl;
	for (unsigned int i = 0; i < x_a.size(); i++)
	{
		std::cout << x_a[i].getID() << std::endl;
	}
	std::cout << "Startowe dane_END" << std::endl;
	//getchar();
	///////////////////////////////////////
	std::vector<Point> x_opt = x_a;
	double Q_a = goalFunction(costMatrix, x_a);
	std::cout << "Q_A: " << Q_a << "            Q_A_END" << std::endl;
	//getchar();
	double Q_opt = Q_a;
	int k = 100;
	double alpha = 0.999;
	//step 2
	while (T > T_min)
	{
		for (int i = 0; i < k; i++)
		{
			//std::vector<Point> x_n = generateNeighbor(x_a);
			double Q_n = goalFunction(costMatrix, x_n);
			double delta = Q_n - Q_a;
			if (delta < 0)
			{
				x_a = x_n;
				Q_a = Q_n;
				if (Q_a < Q_opt)
				{
					x_opt = x_a;
					Q_opt = Q_a;
				}
			}
			else
			{
				double rand = random<double>(0.0, 1.0);
				if (rand < exp(-delta / T)) x_a = x_n;
			}
		}
		T = T * alpha;
	}

	//WYNIKI*/
	return 0;
}
///////////////////////////////////////
thread_local std::mt19937 gen{ std::random_device{}() };

template<typename T>
T random(T min, T max) {
	using dist = std::conditional_t<
		std::is_integral<T>::value,
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>
	>;
	return dist{ min, max }(gen);
}
///////////////////////////////////////
double goalFunction(std::vector<std::vector<int>> costMatrix, std::vector<Point>& route)
{
	std::cout << "FG " << route.size() << std::endl;
	for (unsigned int i = 0; i < route.size(); i++)
	{
		std::cout << route[i].getID() << std::endl;
	}
	std::cout << "FG_END " << route.size() << std::endl;
	double result = 0;
	double beta = 5;
	int delivered = 0;
	int not_taken = 0;
	std::vector<int> towns;
	towns.push_back(route[0].getID());
	for (unsigned int i = 1; i <= route.size() - 1; i++)
	{
		result += costMatrix[route[i - 1].getID()][route[i].getID()];
		towns.push_back(route[i].getID());
	}
	for (unsigned int i = 0; i < route.size() - 1; i++)
	{
		std::vector<Package> package = route[i].getPackages();
		for (unsigned int j = 0; j < package.size(); j++)
		{
			auto it = find(towns.begin(), towns.end(), package[j].getDestination());
			if (it != towns.end())
			{
				delivered++;
			}
			else
			{
				not_taken++;
			}
		}
	}
	return result + beta * not_taken;
}
////////////////////////////////////////////////////////
std::vector<Point> generateStart(int s)
{
	std::vector<Point> start;
	for (int i = 0; i < s; i++)
	{
		start.push_back(Point(random<int>(1, s)));
	}
	return start;
}
/////////////////////////////////////////////////////////
std::vector<std::vector<int>> costMatrixReadCSV()
{
	std::ifstream file("C:/Users/Jakub/Desktop/Studia/MMWD/PackageDelivery/data1.csv");
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
///////////////////////////////////////////////////////





































