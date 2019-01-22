#include "deliverer.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>

#pragma region KONSTRUKTORY
Deliverer::Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_)
	:cost_Matrix(matrix_), org_Packages(packages_)
{
	init(matrix_, packages_);
};
///////////////////////////////////////////
Deliverer::Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_, double t_min, double t_begin, double alfa_, int k_iter, double C1_not_taken)
	:cost_Matrix(matrix_), org_Packages(packages_), T_min(t_min), T_begin(t_begin), alfa(alfa_), k(k_iter), C1(C1_not_taken)
{
	init(matrix_, packages_);
}
Deliverer::~Deliverer()
{
	delete generator;
}
//////////////////////////////////////////////////////////////////////////////
void Deliverer::init(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_)
{
	seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	generator = new std::mt19937(seed);
	//std::cout << "Poczatek konstruktora Deliverer" << std::endl;
	std::vector<std::vector<Package>> list_of_packages(cost_Matrix.size(), std::vector<Package>());
	for (auto pack : org_Packages)
	{
		int source = pack.getSource();
		list_of_packages.at(source).push_back(pack);
	}
	//std::cout << "Srodek konstruktora Deliverer" << std::endl;
	for (int i = 0; i < cost_Matrix.size(); i++)
	{
		points.push_back(Point(i, list_of_packages[i]));
	}
	//std::cout << "Koniec konstruktora Deliverer" << std::endl;
	solution = generate_start_solution();
	best_Solution = solution;
	best_goal_function = goal_Function(solution);
	gf_solution = best_goal_function;
	reset();
}
#pragma endregion
//////////////////////////////////////////
std::vector<int> Deliverer::generate_start_solution()
{
	std::vector<int> result;
	for (int i = 0; i < points.size(); i++)
	{
		insert_Aditional_Waypoint_To_Solution(result,i);
	}
	return result;
}
///////////////////////////////////////////////////////////////////////////////////
std::vector<int> Deliverer::modify_solution(std::vector<int>& currentSolution)
{

		double random = random_01();
		//std::cout << random << std::endl;
		double level = P1_insert_additional_waypoint;
		double level2 = level + P2_erase_one_point;
		double level3 = level2 + P3_swap_points;
		int whereModify;
		if (random<level)
		{
			//std::cout << "INSERT ADDITIONAL WAYPOINT TO SOLUTION" << std::endl;
			whereModify = random_in_range(0, currentSolution.size());
			return insert_Aditional_Waypoint_To_Solution(currentSolution, whereModify);
		}
		else if (random<level2)
		{
			//std::cout << "ERASE ONE POINT" << std::endl;
			whereModify = random_in_range(0, currentSolution.size()-1);
			return erase_one_point(currentSolution, whereModify);
		}
		else if (random<level3)
		{
			//std::cout << "SWAP 2 POINTS" << std::endl;
			return swap_2_points(currentSolution);
		}
		else
		{
			//std::cout << "CHANGE ONE POINT" << std::endl;
			whereModify = random_in_range(0, currentSolution.size()-1);
			return change_one_point(currentSolution, whereModify);
		}

}

///////////////////////////////////////
double Deliverer::goal_Function(std::vector<int> solution_)
{
	reset();
	if (solution_.size() < 2) return C1*org_Packages.size();
	double result = 0;
	int taken = 0;
	std::vector<Package> loadPackage;
	for (int i = 1; i < solution_.size(); i++)
	{
		result += cost_Matrix.at(solution_.at(i-1)).at(solution_.at(i));
	}
	//wersja ³atwiejsza
	/*for (int i=0;i<solution_.size();i++)
	{
		std::cout << "NR: " << solution_.at(i);
		auto packages_in_point = points.at(solution_.at(i)).getPackages();
		for (auto pack : packages_in_point)
		{
			auto dest = std::find(solution_.begin()+i, solution_.end(), pack.getDestination());
			std::cout << " DEST: " << (dest-solution_.begin());
			if (dest != solution_.end())
			{
				std::cout <<" END:"<< (solution_.end() - solution_.begin());
				taken++;
				loadPackage.push_back(pack);
				points.at(solution_.at(i)).deletePackage(pack);
			}
		}
		std::cout << std::endl;
	}*/
	for (auto it = solution_.begin(); it != solution_.end(); it++)
	{
		auto packages_in_point = points.at(*it).getPackages();

		auto ptr = std::find_if(loadPackage.begin(), loadPackage.end(), [it](Package pack) {return pack.getDestination() == *it; });
		while (ptr != loadPackage.end())
		{
			loadPackage.erase(ptr);
			ptr = std::find_if(loadPackage.begin(), loadPackage.end(), [it](Package pack) {return pack.getDestination() == *it; });
		}

		auto part_end = find(it + 1, solution_.end(), *it);
		if (packages_in_point.size() > 0)
		{
			for (auto& pack : packages_in_point)
			{
				if (loadPackage.size() < maxLoad)
				{
					auto dest = find(it, part_end, pack.getDestination());
					if (dest != part_end)
					{
						taken++;
						loadPackage.push_back(pack);
						deletePackage(pack);
					}
				}
			}
			
		}
	}

	
	result += C1 * (org_Packages.size() - taken);
	return result;
}
std::vector<int>::iterator find(std::vector<int>::iterator begin, std::vector<int>::iterator end, int x)
{
	for (;begin!=end; begin++)
	{
		if (*begin == x)
		{
			return begin;
		}
	}
	return end;
}

////////////////////////////////////////////////////
void Deliverer::solve_problem()
{
	best_Solution = solution;
	best_goal_function = goal_Function(best_Solution);
	std::ofstream summaryFile;
	std::ofstream plotFile;
	auto t = std::time(nullptr);
	auto _time = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&_time,"%F-%H-%M");
	std::string timestamp(oss.str());
	std::string filename = "../OUTfiles/summary.txt";
	std::string filename_p = "../OUTfiles/plot.csv";
	filename = filename.insert(19, timestamp);
	filename_p = filename_p.insert(16, timestamp);
	summaryFile.open(filename, std::ofstream::trunc);
	plotFile.open(filename_p, std::ofstream::trunc);

	summaryFile << "# T_min   =  " << T_min << std::endl;
	summaryFile << "# T_begin =  " << T_begin << std::endl;
	summaryFile << "# alfa    =  " << alfa << std::endl;
	summaryFile << "# k       =  " << k << std::endl;
	summaryFile << "# C1      =  " << C1 << std::endl;
	//summaryFile << "# C2      =  " << C2 << std::endl;

	summaryFile.close();
	std::ofstream myfile, file2;
	myfile.open("example.txt");
	file2.open("delta.txt");
	myfile << "Iterator T GF Best_GF" << std::endl;
	int iterator = 0;
	double T = T_begin;
	while (T > T_min)
	{
		
		for (int i = 0; i < k; i++)
		{
			//std::cout << iterator << ": " << T << " - " << i << std::endl;
			std::vector<int> neighbor = modify_solution(solution);
			//printf_sol(neighbor);
			double score = goal_Function(neighbor);
			double delta = score - gf_solution;
			plotFile << best_goal_function << "," << score << std::endl;
			if (delta < 0)
			{
				solution = neighbor;
				gf_solution = score;
				if (score<best_goal_function)
				{
					best_Solution = solution;
					best_goal_function = score;
				}
			}
			else
			{
				double d = random_01();
				if (d < exp(-delta / T))
				{
					solution = neighbor;
				}
				file2 << iterator << " " << exp(-delta / T) << std::endl;
			}
			iterator++;
			myfile << iterator << " " << T << " " << gf_solution << " " << best_goal_function << std::endl;
			
		}
		T *= alfa;
		
	}

	summaryFile.open(filename, std::ofstream::app);
	summaryFile << "BEST SOLUTION:" << std::endl;
	printSolToFile(best_Solution, summaryFile);
	summaryFile << "GOAL FUNCTION:" << std::endl << goal_Function(best_Solution) << std::endl;

	summaryFile.close();
	plotFile.close();
	myfile.close();
	file2.close();
}
/////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
#pragma region FUNKCJE_DO_MODYFIKACJI
//INSERT ADDITIONAL WAYPOINT TO SOLUTION
std::vector<int> Deliverer::insert_Aditional_Waypoint_To_Solution(std::vector<int>& currentSolution, int whereInsert)
{
	
	if (currentSolution.size()==0)
	{
		currentSolution.push_back(random_point());
		return currentSolution;
	}
	else
	{
		int predecessor = whereInsert - 1 < 0 ? -1 : currentSolution.at(whereInsert - 1);
		int successor = whereInsert >= currentSolution.size() ? -1 : currentSolution.at(whereInsert);
		int point_nr;
		do
		{
			point_nr = random_point();
		} while (point_nr == predecessor || point_nr == successor);

		currentSolution.insert(currentSolution.begin() + whereInsert, point_nr);
		return currentSolution;
	}
}
//////////////////////////////////////////////////////////////////
//ERASE ONE POINT
std::vector<int> Deliverer::erase_one_point(std::vector<int>& currentSolution, int whereErese_)
{
	if (whereErese_ < 0 || whereErese_ >= currentSolution.size()) return currentSolution;
	currentSolution.erase(currentSolution.begin() + whereErese_);
	return currentSolution;
}
////////////////////////////////////////////////////////////////////////
// SWAP 2 POINTS
std::vector<int> Deliverer::swap_2_points(std::vector<int>& currentSolution)
{
	int size = currentSolution.size();
	auto it = currentSolution.begin();
	if (size ==2)
	{
		std::iter_swap(it, it + 1);
	}
	else
	{
		int pointOne = random_in_range(0, size-1);
		int pointTwo;
		int a, b, c, d;
		int i = 0;
		do
		{
			if (i == size * 10) return currentSolution;
			pointTwo = random_in_range(0, size-1);
			a = (pointTwo + 1 >= size || pointTwo + 1 == pointOne) ? -1 : currentSolution.at(pointTwo + 1);
			b = (pointTwo - 1 < 0 || pointTwo - 1 == pointOne) ? -2 : currentSolution.at(pointTwo - 1);
			d = (pointOne - 1 < 0 || pointOne - 1 == pointTwo) ? -1 : currentSolution.at(pointOne - 1);
			c = (pointOne + 1 >= size || pointOne + 1 == pointTwo) ? -1 : currentSolution.at(pointOne + 1);
			i++;
		} while (pointTwo == pointOne || a == currentSolution.at(pointOne) || b == currentSolution.at(pointOne)
			|| c == currentSolution.at(pointTwo) || d == currentSolution.at(pointTwo));
		std::iter_swap(it + pointOne, it + pointTwo);
		//std::cout << pointOne << " -> " << pointTwo << std::endl;
	}
	return currentSolution;
}
/////////////////////////////////////////////////////////////////////
// CHANGE ONE POINT
std::vector<int> Deliverer::change_one_point(std::vector<int>& currentSolution, int whereChange_)
{
	if (whereChange_ < 0 || whereChange_ >= currentSolution.size()) return currentSolution;
	int oldPoint = currentSolution.at(whereChange_);
	currentSolution.erase(currentSolution.begin() + whereChange_);

	int predecessor = whereChange_ - 1 < 0 ? -1 : currentSolution.at(whereChange_ - 1);
	int successor = whereChange_ >= currentSolution.size() ? -1 : currentSolution.at(whereChange_);
	int point_nr;
	do
	{
		point_nr = random_point();
	} while (point_nr == predecessor || point_nr == successor || point_nr == oldPoint);

	currentSolution.insert(currentSolution.begin() + whereChange_, point_nr);
	return currentSolution;
}

#pragma endregion
///////////////////////////////////////////////
#pragma region FUNKCJE DODATKOWE
////////////////////////////////////////////////
void Deliverer::deletePackage(Package pack_)
{
	points.at(pack_.getSource()).deletePackage(pack_);
}
////////////////////////////////////////////////
void Deliverer::setStartSolution(std::vector<int> startSolution_)
{
	solution = startSolution_;
}
///////////////////////////////////////////////
Package Deliverer::get_Package_By_Id(int id)
{
	for (auto& pack : org_Packages)
	{
		if (pack.getID() == id) return pack;
	}
}
///////////////////////////////////////////////
void Deliverer::add_Package_By_Pack(Package pack_)
{
	points.at(pack_.getSource()).addPackage(pack_);
}
////////////////////////////////////////////
void Deliverer::add_Package_By_Id(int id_)
{
	Package pack_;
	for (int i = 0; i < org_Packages.size(); i++)
	{
		if (org_Packages.at(i).getID() == id_)
		{
			pack_ = org_Packages.at(i);
			break;
		}
	}
	points.at(pack_.getSource()).addPackage(pack_);
}
////////////////////////////////////////
void Deliverer::reset()
{
	for (auto& point:points)
	{
		point.reset();
	}
}
///////////////////////////////////////////
int Deliverer::random_in_range(int low, int high)
{
	if (low == high || high<0)
	{
		return 0;
	}

	std::uniform_int_distribution<int> distribuation(low, high);
	return distribuation(*generator);
}
///////////////////////////////////////
double Deliverer::random_01()
{
	std::uniform_real_distribution<double> distribuation(0, 1);
	return distribuation(*generator);
}
///////////////////////////////////////////
int Deliverer::random_point()
{
	return random_in_range(0, points.size() - 1);
}
////////////////////////////////////////////////////////////////
bool Deliverer::changeProbabiliti(double P1_, double P2_, double P3_, double P4_)
{
	if (P1_ + P2_ + P3_ + P4_ != 1)return false;
	else
	{
		P1_insert_additional_waypoint = P1_;
		P2_erase_one_point = P2_;
		P3_swap_points = P3_;
		P4_change_one_point = P4_;
		return true;
	}
}
#pragma endregion
///////////////////////////////////////////////////////////////
#pragma region PRINTS

void Deliverer::print_Org_Packages()
{
	std::cout << "Paczki" << std::endl;
	for (auto i : org_Packages)
	{
		std::cout << i.getID() << " : " << i.getSource() << " -> " << i.getDestination() << std::endl;
	}
}
////////////////////////////////////////////////////////////
void Deliverer::print_Cost_Matrix()
{
	std::cout << "COST MATRIX" << std::endl;
	for (auto x : cost_Matrix)
	{
		for (auto y : x)
		{
			std::cout << y << " ";
		}
		std::cout << std::endl;
	}
}
/////////////////////////////////////////////////////////////
void Deliverer::print_Solution()
{
	std::cout << "SOLUTION" << std::endl;
	for (auto x : solution)
	{
		std::cout << x <<" ";
	}
	std::cout << std::endl;
}
/////////////////////////////////////////////////////////////
void Deliverer::printSolToFile(std::vector<int> sol, std::ostream& os)
{
	for (auto inst : sol)
	{
		os << inst << ": ";
	}
	os << std::endl;
}
/////////////////////////////////////////////////////////////
void Deliverer::printf_sol(std::vector<int> sol)
{
	for (auto inst : sol)
	{
		std::cout << inst << " ";
	}
	std::cout << std::endl;
}
////////////////////////////////////////////////////////////
void Deliverer::print_Points()
{
	std::cout << "POINTS" << std::endl;
	for (auto point : points)
	{
		std::cout << point.getID() << ": ";
		for (auto x : point.getPackages())
		{
			std::cout << x.getID() << "[" << x.getSource() << "->" << x.getDestination() << "], ";
		}
		std::cout << std::endl;
	}
}
////////////////////////////////////////////////////////////
void Deliverer::createSummary(std::ofstream& file, std::vector<std::pair<int, std::vector<int>>>& sol)
{

}

#pragma endregion

 

