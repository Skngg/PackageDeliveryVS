#include "point.h"

Point::Point(int Id) :ID(Id)
{ }
/////////////////////////////////////////////////
Point::Point(int Id, std::vector<Package> packages_)
	: ID(Id), originalPackages(packages_)
{ }
/////////////////////////////////////////////////
int Point::getID() { return ID; }
/////////////////////////////////////////////////
std::vector<Package> Point::getOriginalPackages() { return originalPackages; }
std::vector<Package> Point::getAvailablePackages() { return availablePackages; }
//////////////////////////////////////////////////
void Point::addPackage(Package package)
{
	availablePackages.push_back(package);
}
//////////////////////////////////////////////////
void Point::deletePackage(Package package)
{
	for (unsigned int i = 0; i<availablePackages.size() - 1; i++)
	{
		if (availablePackages[i].getID() == package.getID())
		{
			availablePackages.erase(availablePackages.begin() + i);
			break;
		}
	}
}



