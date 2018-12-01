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

	std::vector<Package> load;
	std::vector<Package> delivered;

	std::vector<std::pair<int, std::vector<int>>> solution;
public:
	Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_);
	std::vector<Package> showLoad();
	std::vector<Package> showDelivered();

	//void reset();
	//double goal_Function();
	//void solve_problem();
	void generate_start_solution();
	std::vector<std::pair<int, std::vector<int>>> modify_solution(std::vector<std::pair<int, std::vector<int>>>& currentSolution);
	/*FUNKCJE POMOCNICZE*/
	int random_in_range(int, int);
	double random_01();
};
#endif //DELIVERER_H
