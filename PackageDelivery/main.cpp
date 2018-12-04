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
#include "init.h"

void printf_sol(std::vector<std::pair<int, std::vector<int>>> sol);
///////////////////////////////////////////////
int main()
{
	srand(time(NULL));
	/*USTAWIANIE PARAMETRÓW*/
	double T_0 = 100;
	double T_min = 80;
	double alfa = 0.999;
	int  k = 10;
	double C1 = 60;
	double C2 = 100;
	/////////////////////////////
	std::vector<std::vector<int>> cost_matrix = {
		{0, 1, 1, 100, 1},
		{1, 0, 1, 100, 100},
		{1, 1, 0, 100, 1},
		{100, 100, 100, 0, 100},
		{1, 100, 1,  100, 0}
	};
	std::vector<Package> packages = {
		Package(0,1),
		Package(0,2),
		Package(2,4),
		Package(4,0)
	};
	/////////////////////////////////////
	Deliverer test(cost_matrix, packages);
	test.print_Cost_Matrix();
	test.print_Org_Packages();
	test.print_Points();
	test.print_Solution();
	std::cout << "GF: " << test.goal_Function(test.get_solution()) << std::endl;
	for (int i = 0; i < 5; i++)
	{
		std::cout << "SOLUTION " << i << std::endl;
		printf_sol(test.change_one_point(test.get_solution()));
	}

	
	

	std::cout << "KONIEC PROGRAMU" << std::endl;
	return 0;
}
///////////////////////////////////////
//						std::cout<<<<std::endl;
void printf_sol(std::vector<std::pair<int, std::vector<int>>> sol)
{
	for (auto inst : sol)
	{
		std::cout << inst.first << ": ";
		for (auto pack : inst.second)
		{
			std::cout << pack << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
