#include "point.h"

Point::Point(int Id) :ID(Id)
{ 
	org_packages = std::vector<Package>();
	packages = std::vector<Package>();
}
/////////////////////////////////////////////////
Point::Point(int Id, std::vector<Package> packages_)
	: ID(Id), packages(packages_), org_packages(packages_)
{ }
/////////////////////////////////////////////////
int Point::getID() { return ID; }
/////////////////////////////////////////////////
std::vector<Package> Point::getPackages() { return packages; }
//////////////////////////////////////////////////
void Point::addPackage(Package package)
{
	packages.push_back(package);
}
//////////////////////////////////////////////////
void Point::deletePackage(Package package)
{
	for (unsigned int i = 0; i<packages.size() - 1; i++)
	{
		if (packages[i].getID() == package.getID())
		{
			packages.erase(packages.begin() + i);
			break;
		}
	}
}
////////////////////////////////////////////////
void Point::reset()
{
	packages = org_packages;
}
///////////////////////////////////////////////
Package Point::take_Package(int id)
{
	Package package(0,0);
	for (int i = 0; i < packages.size(); i++)
	{
		if (packages.at(i).getID() == id)
		{
			package = packages[i];
			packages.erase(packages.begin()+i);
			break;
		}
	}
	return package;
}
//////////////////////////////////////////////////
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



