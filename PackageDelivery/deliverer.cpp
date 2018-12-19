#include "deliverer.h"
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <iomanip>

#pragma region KONSTRUKTORY
Deliverer::Deliverer(std::vector<std::vector<int>> matrix_, std::vector<Package> packages_)
	:cost_Matrix(matrix_), org_Packages(packages_)
{
	std::cout<<"Poczatek konstruktora Deliverer"<<std::endl;
	std::vector<std::vector<Package>> list_of_packages(cost_Matrix.size(), std::vector<Package>());
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
	std::vector<std::vector<Package>> list_of_packages(cost_Matrix.size(), std::vector<Package>());
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
#pragma endregion
//////////////////////////////////////////
void Deliverer::generate_start_solution()
{
	int number_of_points = random_in_range(points.size()/2, points.size()+1);

	for (int i = 0; i < number_of_points; i++)
	{
		int point_number = random_in_range(0, points.size());

		int number_of_packages_in_point = points.at(point_number).getPackages().size();

		std::vector<int> packs;

		if (number_of_packages_in_point != 0)
		{
			int number_of_packages = random_in_range(0, number_of_packages_in_point+1);

			for (int j = 1; j <= number_of_packages; j++)
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
	int howModify = random_in_range(0, 6);
		//if (howModify == 0)			// INSERT ADDITIONAL WAYPOINT TO SOLUTION
		//{
		//	std::cout << "INSERT ADDITIONAL WAYPOINT TO SOLUTION"<<std::endl;
		//	return insert_Aditional_Waypoint_To_Solution(currentSolution);
		//}
		//else if (howModify == 1)	// ERASE ONE POINT
		//{
		//	std::cout << "ERASE ONE POINT" << std::endl;
		//	return erase_one_point(currentSolution);
		//}
		//else if (howModify == 2)	// SWAP 2 POINTS
		//{
		//	std::cout << "SWAP 2 POINTS" << std::endl;
		//	return swap_2_points(currentSolution);
		//}
		//else if (howModify == 3)	// MOD PACKAGES TO TAKE
		//{
		//	std::cout << "MOD PACKAGES TO TAKE" << std::endl;
		//	return mod_Packages_To_Take(currentSolution);
		//}
		//else						// CHANGE ONE POINT
		//{
		//	std::cout << "CHANGE ONE POINT" << std::endl;
		//	return change_one_point(currentSolution);
		//}
		double random = random_01();
		double level = P1_insert_additional_waypoint;
		double level2 = level + P2_erase_one_point;
		double level3 = level2 + P3_swap_points;
		double level4 = level3 + P4_mod_packages;
		if (random<level)
		{
			std::cout << "INSERT ADDITIONAL WAYPOINT TO SOLUTION" << std::endl;
			int whereInsert = random_in_range(0, currentSolution.size());
			return insert_Aditional_Waypoint_To_Solution(currentSolution, whereInsert);
		}
		else if (random<level2)
		{
			std::cout << "ERASE ONE POINT" << std::endl;
			return erase_one_point(currentSolution);
		}
		else if (random<level3)
		{
			std::cout << "SWAP 2 POINTS" << std::endl;
			return swap_2_points(currentSolution);
		}
		else if (random < level4)
		{
			std::cout << "MOD PACKAGES TO TAKE" << std::endl;
			return mod_Packages_To_Take(currentSolution);
		}
		else
		{
			std::cout << "CHANGE ONE POINT" << std::endl;
			return change_one_point(currentSolution);
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
	result += (C2 * taken + C1 * (org_Packages.size() - delivered - taken));
	return result;
}

////////////////////////////////////////////////////
void Deliverer::solve_problem()
{
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
	summaryFile << "# C2      =  " << C2 << std::endl;

	summaryFile.close();


	int iterator = 0;
	double T = T_begin;
	while (T > T_min)
	{
		plotFile << goal_Function(solution) << std::endl;
		for (int i = 0; i < k; i++)
		{
			std::cout << iterator << ": " << T << " - " << i << std::endl;
			std::vector<std::pair<int, std::vector<int>>> neighbor = modify_solution(solution);

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
			iterator++;
		}
		T *= alfa;
	}

	summaryFile.open(filename, std::ofstream::app);
	summaryFile << "BEST SOLUTION:" << std::endl;
	printSolToFile(best_Solution, summaryFile);

	summaryFile.close();
	plotFile.close();
}
/////////////////////////////////////////////////
#pragma region FUNKCJE DODATKOWE
////////////////////////////////////////////////
Package Deliverer::get_Package_By_Id(int id)
{
	for (auto pack : org_Packages)
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
int Deliverer::random_in_range(int low, int high)
{
	if (low==0 && high == 0)
	{
		return 0;
	}
	return low + rand() % (high - low);
}
///////////////////////////////////////
double Deliverer::random_01()
{
	double x = rand();
	return x / RAND_MAX;
}
////////////////////////////////////////
void Deliverer::reset()
{
	for (auto point : points)
	{
		point.reset();
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
		std::cout << x.first << ": ";
		for (auto y : x.second)
		{
			std::cout << y << " ";
		}
		std::cout << std::endl;
	}
}
/////////////////////////////////////////////////////////////
void Deliverer::printSolToFile(std::vector<std::pair<int, std::vector<int>>> sol, std::ostream& os)
{
	for (auto inst : sol)
	{
		os << inst.first << ": ";
		for (auto pack : inst.second)
		{
			std::cout << pack << " ";
		}
		os << std::endl;
	}
	os << std::endl;
}
/////////////////////////////////////////////////////////////
void Deliverer::printf_sol(std::vector<std::pair<int, std::vector<int>>> sol)
{
	for (auto inst : sol)
	{
		std::cout << inst.first << ": ";
		for (auto pack : inst.second)
		{
			std::cout << pack << " ";
		}
		std::cout << std::endl;
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
		for (auto x : point.getOrgPackages())
		{
			std::cout << x.getID() << "[" << x.getSource() << "->" << x.getDestination() << "], ";
		}
		std::cout << std::endl;
	}
}
////////////////////////////////////////////////////////////
void createSummary(std::ofstream& file, std::vector<std::pair<int, std::vector<int>>> sol)
{

}

#pragma endregion
///////////////////////////////////////////////////////////////
#pragma region FUNKCJE_DO_MODYFIKACJI
//INSERT ADDITIONAL WAYPOINT TO SOLUTION
std::vector<std::pair<int, std::vector<int>>> Deliverer::insert_Aditional_Waypoint_To_Solution(std::vector<std::pair<int, std::vector<int>>>& currentSolution, int whereInsert)
{
	std::vector<std::pair<int, std::vector<int>>> newSolution = currentSolution;
	std::vector<std::pair<int, std::vector<int>>>::iterator it = newSolution.begin();

	int number_of_points = points.size();
	int newWaypoint;
	int a = (whereInsert >= currentSolution.size()) ? -1 : currentSolution.at(whereInsert).first;
	int b = (whereInsert - 1 < 0) ? -1 : currentSolution.at(whereInsert - 1).first;
	do
	{
		newWaypoint = random_in_range(0, number_of_points);

	} while (newWaypoint == a || newWaypoint == b);

	int number_of_packages_in_point = points.at(newWaypoint).getPackages().size();

	std::vector<int> packs;
	if (number_of_packages_in_point != 0)
	{

		int number_of_packages = random_in_range(0, number_of_packages_in_point + 1);
		for (int j = 1; j <= number_of_packages; j++)
		{
			int which_package = random_in_range(0, points.at(newWaypoint).getPackages().size());
			int id = points.at(newWaypoint).getPackages().at(which_package).getID();
			packs.push_back(id);
			points.at(newWaypoint).delete_Package_By_Id(id);
		}
	}
	std::pair<int, std::vector<int>> newInstruction = std::pair<int, std::vector<int>>(newWaypoint, packs);

	newSolution.insert(it + whereInsert, newInstruction);
	std::cout << "Po zmianie" << std::endl;
	printf_sol(newSolution);
	return newSolution;
}
//////////////////////////////////////////////////////////////////
//ERASE ONE POINT
std::vector<std::pair<int, std::vector<int>>> Deliverer::erase_one_point(std::vector<std::pair<int, std::vector<int>>>& currentSolution)
{

	int number_of_points = currentSolution.size();
	if (number_of_points == 0) return currentSolution;
	int a, b;
	int whereErase;
	do
	{
		whereErase = random_in_range(0, number_of_points);
		a = (whereErase + 1 >= number_of_points) ? -1 : currentSolution.at(whereErase + 1).first;
		b = (whereErase -1 < 0) ? -2 : currentSolution.at(whereErase - 1).first;
	} while (a==b);

	for (int i = 0; i < currentSolution.at(whereErase).second.size(); i++)
	{
		add_Package_By_Id(currentSolution.at(whereErase).second.at(i));
	}
	currentSolution.erase(currentSolution.begin() + whereErase);

	std::cout << "Po zmianie" << std::endl;
	printf_sol(currentSolution);
	return currentSolution;
}
////////////////////////////////////////////////////////////////////////
// SWAP 2 POINTS
std::vector<std::pair<int, std::vector<int>>> Deliverer::swap_2_points(std::vector<std::pair<int, std::vector<int>>>& currentSolution)
{
	std::vector<std::pair<int, std::vector<int>>> newSolution = currentSolution;
	std::vector<std::pair<int, std::vector<int>>>::iterator it = newSolution.begin();


	int number_of_points = currentSolution.size();
	if (number_of_points < 2) return currentSolution;
	int pointOne = random_in_range(0, number_of_points);
	int pointTwo;
	int a, b, c, d;
	int i = 0;
	do
	{
		if (i == number_of_points * 10) return currentSolution;
		pointTwo = random_in_range(0, number_of_points);
		a = (pointTwo + 1 >= number_of_points || pointTwo+1 == pointOne) ? -1 : currentSolution.at(pointTwo + 1).first;
		b = (pointTwo - 1 < 0 || pointTwo - 1 == pointOne) ? -2 : currentSolution.at(pointTwo - 1).first;
		d = (pointOne - 1 < 0 || pointOne - 1 == pointTwo) ? -1 : currentSolution.at(pointOne - 1).first;
		c = (pointOne + 1 >= number_of_points || pointOne + 1 == pointTwo) ? -1 : currentSolution.at(pointOne + 1).first;
		i++;
	} while (pointTwo == pointOne || a==currentSolution.at(pointOne).first || b== currentSolution.at(pointOne).first 
		|| c == currentSolution.at(pointTwo).first || d == currentSolution.at(pointTwo).first);

	std::iter_swap(it + pointOne, it + pointTwo);
	std::cout << "Po zmianie" << std::endl;
	printf_sol(newSolution);
	return newSolution;
}
/////////////////////////////////////////////////////////////////////
//MOD PACKAGES TO TAKE
std::vector<std::pair<int, std::vector<int>>> Deliverer::mod_Packages_To_Take(std::vector<std::pair<int, std::vector<int>>>& currentSolution)
{
	return mod_Package_At(currentSolution, random_in_range(0, currentSolution.size()));

	//std::vector<std::pair<int, std::vector<int>>> newSolution = currentSolution;
	//std::vector<std::pair<int, std::vector<int>>>::iterator it = newSolution.begin();


	//int number_of_points = newSolution.size();

	//int whereModify = random_in_range(0, number_of_points);
	//int point_number = newSolution.at(whereModify).first;
	//int number_of_packages_in_point = points.at(point_number).getOrgPackages().size();
	//std::cout << "Nr point " << point_number << std::endl;
	//if (number_of_packages_in_point != 0)
	//{
	//	std::vector<int> newPacks = newSolution.at(point_number).second;   // COPY CURRENT PACKS FOR MODIFICATION

	//	int whichPackModify = random_in_range(0, number_of_packages_in_point);
	//	int id = points.at(point_number).getOrgPackages().at(whichPackModify).getID();
	//	std::cout << "Nr point " << point_number << " ,id pack: " << id << std::endl;
	//	
	//	if (std::find(newPacks.begin(), newPacks.end(), id) != newPacks.end())
	//	{
	//		std::cout << "Pierwszy if "  << std::endl;
	//		// ---IF currentPacks CONTAINS whickPackModify---
	//		for (int i=0;i< points.at(point_number).getPackages().size();i++)
	//		{
	//			if (points.at(point_number).getPackages().at(i).getID() == id)
	//			{
	//				std::cout << "odejmij" << std::endl;
	//				points.at(newSolution.at(point_number).first).add_Package_By_Id(id);
	//				newPacks.erase(newPacks.begin() + whereModify);
	//				break;
	//			}
	//		}
	//		
	//	}
	//	else
	//	{
	//		std::cout << "DODAJ " << std::endl;
	//		points.at(newSolution.at(point_number).first).delete_Package_By_Id(id);
	//		newPacks.push_back(id);
	//	}
	//	newSolution.at(whereModify).second = newPacks;
	//}
	
	//return newSolution;
}
/////////////////////////////////////////////////////////////////////
// CHANGE ONE POINT
std::vector<std::pair<int, std::vector<int>>> Deliverer::change_one_point(std::vector<std::pair<int, std::vector<int>>>& currentSolution)
{
	if (currentSolution.size() == 0) return currentSolution;
	std::vector<std::pair<int, std::vector<int>>> newSolution = currentSolution;
	std::vector<std::pair<int, std::vector<int>>>::iterator it = newSolution.begin();

	int whereModify = random_in_range(0, newSolution.size());
	int newWaypoint;
	int a = (whereModify + 1 >= currentSolution.size()) ? -1 : currentSolution.at(whereModify + 1).first;
	int b = (whereModify - 1 < 0) ? -1 : currentSolution.at(whereModify - 1).first;

	do
	{
		newWaypoint = random_in_range(0, get_points().size());

	} while (a == newWaypoint || b == newWaypoint);


	for (int i = 0; i < newSolution.at(whereModify).second.size(); i++)
	{
		add_Package_By_Id(newSolution.at(whereModify).second.at(i));
	}
	newSolution.erase(it + whereModify);

	int number_of_packages_in_point = points.at(newWaypoint).getPackages().size();

	std::vector<int> packs;
	if (number_of_packages_in_point != 0)
	{
		int number_of_packages = random_in_range(0, number_of_packages_in_point + 1);

		for (int j = 1; j <= number_of_packages; j++)
		{
			int which_package = random_in_range(0, points.at(newWaypoint).getPackages().size());
			int id = points.at(newWaypoint).getPackages().at(which_package).getID();
			packs.push_back(id);
			points.at(newWaypoint).delete_Package_By_Id(id);			
		}
	}
	std::pair<int, std::vector<int>> newInstruction = std::pair<int, std::vector<int>>(newWaypoint, packs);
	
	newSolution.insert(newSolution.begin() + whereModify, newInstruction);
	std::cout << "Po zmianie" << std::endl;
	printf_sol(newSolution);

	return newSolution;

}

#pragma endregion

std::vector<std::pair<int, std::vector<int>>> Deliverer::mod_Package_At(std::vector<std::pair<int, std::vector<int>>>& currentSolution, int where_)
{
	if (currentSolution.size() == 0) return currentSolution;
	int NR_point = currentSolution.at(where_).first;

	int number_of_all_packages = points.at(NR_point).getOrgPackages().size();
	if (number_of_all_packages != 0)
	{
		std::vector<int> packs = currentSolution.at(where_).second;

		int it_pack = random_in_range(0, number_of_all_packages);

		int Id_pack = points.at(NR_point).getOrgPackages().at(it_pack).getID();

		if (std::find(packs.begin(),packs.end(),Id_pack) != packs.end())
		{	//Jeœli znaleziono w punkcie
			points.at(NR_point).add_Package_By_Id(Id_pack);
			for (int i = 0; i<packs.size();i++)
			{
				if (packs.at(i) == Id_pack)
				{
					packs.erase(packs.begin() + i);
					break;
				}
			}
		}
		else
		{	//jeœli nie znaleziono w punkcie
			for (int i = 0; i < points.at(NR_point).getPackages().size(); i++)
			{
				if (points.at(NR_point).getPackages().at(i).getID() == Id_pack)
				{
					points.at(NR_point).delete_Package_By_Id(Id_pack);
					packs.push_back(Id_pack);
					break;
				}
			}
		}
		currentSolution.at(where_).second = packs;
	}
	std::cout << "Po zmianie" << std::endl;
	printf_sol(currentSolution);
	return currentSolution;
}

 

