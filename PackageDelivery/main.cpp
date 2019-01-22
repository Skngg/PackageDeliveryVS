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


/*******************MAIN**********************/
int main()
{
	srand(time(NULL));


	/*USTAWIANIE PARAMETRÓW*/
	double T_0 = 1;
	double T_min = 0.001;
	double alfa = 0.999;
	int  k = 1;
	double C1 = 50;
	/////////////////////////////
	std::vector<std::vector<int>> cost_matrix = {
		{5, 1, 20, 20, 1},
		{1, 5, 1, 20, 20},
		{20, 1, 5, 20, 1},
		{20, 20, 20, 5, 20},
		{1, 20, 1,  20, 5}
	};
	std::vector<Package> packages = {
		Package(0,1),
		Package(0,2),
		Package(2,4),
		Package(4,0)
	};
	/////////////////////////////////////
	Deliverer test(cost_matrix, packages, T_min, T_0, alfa, k, C1);
	test.setMaxLoad(1);
	std::vector<int> x = { 4,0,4,2,4,2,4 };
	test.print_Cost_Matrix();
	test.print_Org_Packages();
	test.print_Points();
	
	test.setStartSolution(test.generate_start_solution());
	test.print_Solution();
	std::cout << "GF: " << test.goal_Function(test.get_solution()) << std::endl;

	clock_t begin = clock();
	test.solve_problem();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << std::endl << "BEST SOLUTION" << std::endl;
	test.printf_sol(test.get_best_Solution());
	std::cout << "GOAL FUNCTION: " << test.goal_Function(test.get_best_Solution())<< std::endl;
	std::cout << "TIME: " << elapsed_secs << " [s]" << std::endl;
	

	std::cout << "KONIEC PROGRAMU" << std::endl;
	return 0;
}
///////////////////////////////////////



