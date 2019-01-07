#include "point.h"
#include <algorithm>

Point::Point(int Id) 
:ID(Id),org_packages(std::vector<Package>()), packages(std::vector<Package>())
{}
/////////////////////////////////////////////////
Point::Point(int Id, std::vector<Package> packages_)
	: ID(Id), packages(packages_), org_packages(packages_)
{ }
/////////////////////////////////////////////////
int Point::getID()const { return ID; }
/////////////////////////////////////////////////
std::vector<Package> Point::getOrgPackages()const { return org_packages; }
std::vector<Package> Point::getPackages() { return packages; }
//////////////////////////////////////////////////
void Point::addPackage(Package package)
{
	packages.push_back(package);
}
///////////////////////////////////////////////
void Point::add_Package_By_Id(int id)
{
	for (int i = 0; i < org_packages.size(); i++)
	{
		if (org_packages.at(i).getID() == id)
		{
			packages.push_back(org_packages.at(i));
			return;
		}
	}
}
//////////////////////////////////////////////////
void Point::deletePackage(Package& package_)
{
	for (unsigned int i = 0; i<packages.size(); i++)
	{
		if (packages.at(i).getID() == package_.getID())
		{
			packages.erase(packages.begin() + i);
			break;
		}
	}	
}
////////////////////////////////////////////////
void Point::reset()
{
	packages.clear();
	for (auto& pack : org_packages)
	{
		packages.push_back(pack);
	}

}
///////////////////////////////////////////////
void Point::delete_Package_By_Id(int id)
{
	for (int i = 0; i < packages.size(); i++)
	{
		if (packages.at(i).getID() == id)
		{
			packages.erase(packages.begin() + i);
			return;
		}
	}
}
