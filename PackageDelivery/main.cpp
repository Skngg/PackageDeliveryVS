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
	double T_0 = 10;
	double T_min = 0.0001;
	double alfa = 0.999;
	int  k = 4;
	double C1 = 1000;
	int maxLoad = 20;
	

	std::vector<Package> packages;
	std::vector<std::vector<int>> cost_matrix = costMatrixReadCSV();
	initFromFile(packages);


	Deliverer test(cost_matrix, packages, T_min, T_0, alfa, k, C1,maxLoad);
	test.changeProbabiliti(0.35, 0.35, 0.15, 0.15); 

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
	std::cout << "NUMBER OF DELIVERED PACKAGES: " << test.how_many_pack_delivered << " / " << packages.size() << std::endl;
	std::cout << "GOAL FUNCTION: " << test.goal_Function(test.get_best_Solution())<< std::endl;
	std::cout << "TIME: " << elapsed_secs << " [s]" << std::endl;
	

	std::cout << "KONIEC PROGRAMU" << std::endl;
	return 0;
}
///////////////////////////////////////



