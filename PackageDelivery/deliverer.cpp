#include "deliverer.h"
#include <stdlib.h>
#include <ctime>
#include <algorithm>

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
				packs.push_back(points.at(point_number).getPackages().at(which_package).getID());
				points.at(point_number).delete_Package_By_Id(points.at(point_number).getPackages().at(which_package).getID());
			}
		}
		solution.push_back(std::pair<int, std::vector<int>>(point_number, packs));
		
	}
}

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


}
////////////////////////////////////////
/* FUNKCJE POMOCNICZE */
////////////////////////////////////////
int Deliverer::random_in_range(int low, int high)
{
	return low + rand() % (high - low + 1);
}
///////////////////////////////////////
double Deliverer::random_01()
{
	return rand() / RAND_MAX;
}

