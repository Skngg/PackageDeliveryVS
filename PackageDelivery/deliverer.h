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
	double T_min = 80;
	double T_begin = 100;
	double alfa = 0.999;
	int k = 10;
	double C1 = 50;
	double C2 = 100;



	std::vector<std::pair<int, std::vector<int>>> solution;
	std::vector<std::pair<int, std::vector<int>>> best_Solution;
	double best_goal_function;
public:
	Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_, double t_min, double t_begin, double alfa, int k_iter, double C1_not_taken, double taken);
	Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_);


	void reset();
	double goal_Function(std::vector<std::pair<int, std::vector<int>>>);
	void solve_problem();
	void generate_start_solution();
	std::vector<std::pair<int, std::vector<int>>> modify_solution(std::vector<std::pair<int, std::vector<int>>>& currentSolution);
	int random_in_range(int, int);
	double random_01();
	Package get_Package_By_Id(int id);

	std::vector<Package> get_org_Packages() { return org_Packages; };
	std::vector<std::vector<int>> get_cost_Matrix() { return cost_Matrix; };
	std::vector<Point> get_points() { return points; };
	std::vector<std::pair<int, std::vector<int>>> get_solution() { return solution; };
	std::vector<std::pair<int, std::vector<int>>> get_best_Solution() { return best_Solution; };
};
#endif //DELIVERER_H
