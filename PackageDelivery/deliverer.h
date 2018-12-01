#ifndef DELIVERER_H
#define DELIVERER_H
#include "package.h"
#include "point.h"
#include <vector>


class Deliverer
{
private:
	std::vector<Point> route;
	std::vector<Package> load;
	std::vector<Package> delivered;
public:
	Deliverer();
	std::vector<Point> showRoute();
	std::vector<Package> showLoad();
	std::vector<Package> showDelivered();

	void takePackage(Package newPackage);
	void deliverPackage(Package package);
	void newRoute(std::vector<Point> newRoute);
};
#endif //DELIVERER_H
