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
	double T_0 = 1000;
	double T_min = 0.0001;
	double alfa = 0.999;
	int  k = 4;
	double C1 = 400;
	int maxLoad = 40;

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
	auto t = std::time(nullptr);
	auto _time = *std::localtime(&t);
	clock_t begin = clock();
	test.solve_problem();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << std::endl << "BEST SOLUTION" << std::endl;
	test.printf_sol(test.get_best_Solution());
	std::cout << "NUMBER OF DELIVERED PACKAGES: " << test.how_many_pack_delivered << " / " << packages.size() << std::endl;
	std::cout << "GOAL FUNCTION: " << test.goal_Function(test.get_best_Solution())<< std::endl;
	std::cout << "TIME: " << elapsed_secs << " [s]" << std::endl;
	
	std::cout << "OUTPUT FILES:" << std::endl;
	std::cout << "/OutputFiles/plot" << std::put_time(&_time, "%H-%M") << ".csv" << std::endl;
	std::cout << "/OutputFiles/summary" << std::put_time(&_time, "%H-%M") << ".txt" << std::endl;
	std::cout << "KONIEC PROGRAMU" << std::endl;
	return 0;
}
///////////////////////////////////////



