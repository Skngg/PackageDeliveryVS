#ifndef DELIVERER_H
#define DELIVERER_H
#include "package.h"
#include "point.h"
#include <vector>
#include "init.h"
#include <random>

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
	double C1 = 25;
	int maxLoad = 5;

	double P1_insert_additional_waypoint = 0.10;
	double P2_erase_one_point = 0.15;
	double P3_swap_points = 0.35;
	double P4_change_one_point = 0.40;

	long long seed;
	std::mt19937* generator;

	std::vector<int> solution;
	std::vector<int> best_Solution;
	double best_goal_function;
	double gf_solution;

public:
	Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_, double t_min, double t_begin, double alfa, int k_iter, double C1_not_taken);
	Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_);
	void init(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_);
	~Deliverer();

	void solve_problem();
	double goal_Function(std::vector<int>);
	std::vector<int> modify_solution(std::vector<int>& currentSolution);
	std::vector<int> generate_start_solution();
	
	 void setTmin(double tmin_) { T_min = tmin_; }
	 void setTbegin(double tbegin) { T_begin = tbegin; }
	 void setAlfa(double alfa_) { alfa = alfa_; }
	 void setK(int k_) { k = k_; }
	 void setCostNotTakenPackage(double c1_) { C1 = c1_; }
	 void setMaxLoad(int load) { maxLoad = load; }

	 bool changeProbabiliti(double, double, double, double);

	Package get_Package_By_Id(int id);
	void add_Package_By_Pack(Package pack_);
	void add_Package_By_Id(int id_);
	void reset();
	void deletePackage(Package);
	//GET//////////////////////////////////////////////////////////////////
	std::vector<Package> get_org_Packages() { return org_Packages; };
	std::vector<std::vector<int>> get_cost_Matrix() { return cost_Matrix; };
	std::vector<Point> get_points() { return points; };
	std::vector<int> get_solution() { return solution; };
	std::vector<int> get_best_Solution() { return best_Solution; };
	void setStartSolution(std::vector<int>);
	///MODIFY SOLUTION/////////////////////////////////////////////////////////////
	std::vector<int> insert_Aditional_Waypoint_To_Solution(std::vector<int>& currentSolution, int location);
	std::vector<int> erase_one_point(std::vector<int>& currentSolution,int whereErese);
	std::vector<int> swap_2_points(std::vector<int>& currentSolution);
	std::vector<int> change_one_point(std::vector<int>& currentSolution,int whereChange_);
	//PRINT///////////////////////////////////////////////////////////////
	void print_Org_Packages();
	void print_Cost_Matrix();
	void print_Solution();
	void print_Points();
	void printf_sol(std::vector<int> sol);
	void printSolToFile(std::vector<int> sol, std::ostream& os);
	////////RANDOM///////////////////////
	int random_in_range(int, int);
	double random_01();
	int random_point();
	
	/////////////////////////////////////////////////////////////////////////
	void createSummary(std::ofstream& file, std::vector<std::pair<int, std::vector<int>>>& sol);


};
#endif //DELIVERER_H
