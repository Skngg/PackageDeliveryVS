#include "point.h"

Point::Point(int Id) :ID(Id)
{ }
/////////////////////////////////////////////////
Point::Point(int Id, std::vector<Package> packages_)
	: ID(Id), packages(packages_)
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



