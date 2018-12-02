#include "deliverer.h"
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <iostream>

Deliverer::Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_)
	:cost_Matrix(matrix_), org_Packages(packages_)
{
	std::cout<<"Poczatek konstruktora Deliverer"<<std::endl;
	std::vector<std::vector<Package>> list_of_packages(cost_Matrix.size(), std::vector<Package>(cost_Matrix.size()));
	for (auto pack : org_Packages)
	{
		int source = pack.getSource();
		list_of_packages.at(source).push_back(pack);
	}
	std::cout<<"Srodek konstruktora Deliverer"<<std::endl;
	for (int i = 0; i < cost_Matrix.size(); i++)
	{
		points.push_back(Point(i, list_of_packages[i]));
	}
	reset();
	generate_start_solution();
	best_Solution = solution;
	best_goal_function = goal_Function(solution);
	std::cout<<"Koniec konstruktora Deliverer"<<std::endl;
};
///////////////////////////////////////////
Deliverer::Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_, double t_min, double t_begin, double alfa_, int k_iter, double C1_not_taken, double C2_taken)
	:cost_Matrix(matrix_), org_Packages(packages_), T_min(t_min), T_begin(t_begin), alfa(alfa_), k(k_iter), C1(C1_not_taken), C2(C2_taken)
{
	std::vector<std::vector<Package>> list_of_packages;
	for (auto pack : org_Packages)
	{
		int source = pack.getSource();
		list_of_packages[source].push_back(pack);
	}

	for (int i = 0; i < cost_Matrix.size(); i++)
	{
		points.push_back(Point(i, list_of_packages[i]));
	}
	reset();
	generate_start_solution();
	best_Solution = solution;
	best_goal_function = goal_Function(solution);
}

//////////////////////////////////////////
void Deliverer::generate_start_solution()
{
	int number_of_points = random_in_range(2, points.size());

	for (int i = 0; i < number_of_points; i++)
	{
		int point_number = random_in_range(0, points.size());

		int number_of_packages_in_point = points.at(point_number).getPackages().size();

		std::vector<int> packs;

		if (number_of_packages_in_point != 0)
		{
			int number_of_packages = random_in_range(0, number_of_packages_in_point);

			for (int j = 0; j < number_of_packages; j++)
			{
				int which_package = random_in_range(0, points.at(point_number).getPackages().size());
				int id = points.at(point_number).getPackages().at(which_package).getID();
				packs.push_back(id);

				points.at(point_number).delete_Package_By_Id(id);
			}
		}
		solution.push_back(std::pair<int, std::vector<int>>(point_number, packs));
		
	}
}
///////////////////////////////////////////////////////////////////////////////////
std::vector<std::pair<int, std::vector<int>>> Deliverer::modify_solution(std::vector<std::pair<int, std::vector<int>>>& currentSolution)
{
	std::vector<std::pair<int, std::vector<int>>> newSolution = currentSolution;
	std::vector<std::pair<int, std::vector<int>>>::iterator it = newSolution.begin();

	int howModify = random_in_range(0, 4);
	int number_of_points = points.size();



	if (howModify == 0)			// INSERT ADDITIONAL WAYPOINT TO SOLUTION
	{
		int newWaypoint = random_in_range(0, number_of_points);
		int whereInsert = random_in_range(0, number_of_points);
		int number_of_packages_in_point = points.at(newWaypoint).getPackages().size();

		std::vector<int> packs;
		if (number_of_packages_in_point != 0)
		{


			int number_of_packages = random_in_range(0, number_of_packages_in_point);
			for (int j = 0; j < number_of_packages; j++)
			{
				int which_package = random_in_range(0, points.at(newWaypoint).getPackages().size());
				packs.push_back(points.at(newWaypoint).getPackages().at(which_package).getID());
				points.at(newWaypoint).delete_Package_By_Id(points.at(newWaypoint).getPackages().at(which_package).getID());
			}
		}
		std::pair<int, std::vector<int>> newInstruction = std::pair<int, std::vector<int>>(newWaypoint, packs);

		newSolution.insert(it + whereInsert, newInstruction);
	}
	else if (howModify == 1)	// ERASE ONE POINT
	{
		int whereErase = random_in_range(0, number_of_points);

		newSolution.erase(it + whereErase);
	}
	else if (howModify == 2)	// SWAP 2 POINTS
	{
		int pointOne = random_in_range(0, number_of_points);
		int pointTwo;
		do
		{
			int pointTwo = random_in_range(0, number_of_points);
		} while (pointTwo == pointOne);

		std::iter_swap(it + pointOne, it + pointTwo);
	}
	else if (howModify == 3)	// MOD PACKAGES TO TAKE
	{
		int whereModify = random_in_range(0, number_of_points);
		int number_of_packages_in_point = points.at(whereModify).getPackages().size();
		
		if (number_of_packages_in_point != 0)
		{
			std::vector<int> newPacks = std::get<1>(newSolution.at(whereModify)); // COPY CURRENT PACKS FOR MODIFICATION
			int whichPackModify = random_in_range(0, number_of_packages_in_point);
			if (std::find(newPacks.begin(), newPacks.end(), whichPackModify) != newPacks.end())
			{
				// ---IF currentPacks CONTAINS whickPackModify---
				newPacks.erase(newPacks.begin() + whichPackModify);
			}
			else
			{
				newPacks.push_back(whichPackModify);
			}
		}	
	}
	else						// CHANGE ONE POINT
	{
		int whereModify = random_in_range(0, number_of_points);
		int newWaypoint = random_in_range(0, number_of_points);

		//		TBD
	}
	return newSolution;

}
////////////////////////////////////////
int Deliverer::random_in_range(int low, int high)
{
	return low + rand() % (high - low);
}
///////////////////////////////////////
double Deliverer::random_01()
{
	return rand() / RAND_MAX;
}
////////////////////////////////////////
void Deliverer::reset()
{
	for (auto point : points)
	{
		point.reset();
	}
}
///////////////////////////////////////
double Deliverer::goal_Function(std::vector<std::pair<int, std::vector<int>>> sol)
{
	double result = 0;
	int delivered = 0;
	int taken = 0;
	std::vector<int> towns;
	for (int i = 0; i < sol.size(); i++)
	{
		if (i!=0)
		{
			result += cost_Matrix[sol[i - 1].first][sol[i].first];
		}
		towns.push_back(sol[i].first);
	}
	for (int i = 0; i < sol.size(); i++)
	{
		for (auto pack_id : sol[i].second)
		{
			auto it = find(towns.begin() + i, towns.end(), get_Package_By_Id(pack_id).getDestination());
			if (it != towns.end())
			{
				delivered++;
			}
			else
			{
				taken++;
			}
		}
	}
	result -= (C2 * taken + C1 * (org_Packages.size() - delivered - taken));
	return result;
}
/////////////////////////////////////////////////
Package Deliverer::get_Package_By_Id(int id)
{
	for (auto pack : org_Packages)
	{
		if (pack.getID() == id) return pack;
	}
}
////////////////////////////////////////////////////
void Deliverer::solve_problem()
{
	double T = T_begin;
	while (T > T_min)
	{
		for (int i = 0; i < k; i++)
		{
			std::vector<std::pair<int, std::vector<int>>> neighbor;
			double delta = goal_Function(neighbor) - goal_Function(solution);

			if (delta < 0)
			{
				solution = neighbor;
				double score = goal_Function(solution);
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
			}
		}
		T *= alfa;
	}
}
//////////////////////////////////////////////////////////////
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
		std::cout << x.first << ": ";
		for (auto y : x.second)
		{
			std::cout << y << " ";
		}
		std::cout << std::endl;
	}
}

