#ifndef DELIVERER_H
#define DELIVERER_H
#include "package.h"
#include "point.h"
#include <vector>


class Deliverer
{
private:
	std::vector<Package> org_Packages;
	std::vector<std::vector<int>> cost_Matrix;
	std::vector<Point> points;
	double T_min = 20;
	double T_begin = 100;
	double alfa = 0.999;
	int k = 10;
	double C1;
	double C2;

	std::vector<Package> load;
	std::vector<Package> delivered;

	std::vector<std::pair<int, std::vector<int>>> solution;
	std::vector<std::pair<int, std::vector<int>>> best_Solution;
	double best_goal_function;
public:
	Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_);
	std::vector<Package> showLoad();
	std::vector<Package> showDelivered();

	void reset();
	double goal_Function(std::vector<std::pair<int, std::vector<int>>>);
	void solve_problem();
	void generate_start_solution();
	std::vector<std::pair<int, std::vector<int>>> modify_solution(std::vector<std::pair<int, std::vector<int>>>& currentSolution);
	int random_in_range(int, int);
	double random_01();
	Package get_Package_By_Id(int id);
};
#endif //DELIVERER_H
