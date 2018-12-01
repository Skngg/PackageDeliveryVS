#include "deliverer.h"

Deliverer::Deliverer() {};
/////////////////////////////////////////
std::vector<Point> Deliverer::showRoute() { return route; }
std::vector<Package> Deliverer::showLoad() { return load; }
std::vector<Package> Deliverer::showDelivered() { return delivered; }
//////////////////////////////////////////
void Deliverer::takePackage(Package newPackage)
{
	load.push_back(newPackage);
}
//////////////////////////////////////////
void Deliverer::deliverPackage(Package package)
{
	delivered.push_back(package);
	for (unsigned int i = 0; i<load.size() - 1; i++)
	{
		if (load[i].getID() == package.getID())
		{
			load.erase(load.begin() + i);
			break;
		}
	}
}
//////////////////////////////////////////
void Deliverer::newRoute(std::vector<Point> newRoute)
{
	route = newRoute;
}
