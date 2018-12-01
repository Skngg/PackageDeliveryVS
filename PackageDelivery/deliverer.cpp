#include "deliverer.h"
#include <stdlib.h>
#include <ctime>

Deliverer::Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_)
	:cost_Matrix(matrix_), org_Packages(packages_)
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
	generate_start_solution();
	best_Solution = solution;
	best_goal_function = goal_Function(solution);
};
/////////////////////////////////////////
std::vector<Package> Deliverer::showLoad() { return load; }
std::vector<Package> Deliverer::showDelivered() { return delivered; }
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

