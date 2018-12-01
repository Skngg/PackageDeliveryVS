#ifndef POINT_H
#define POINT_H
#include "package.h"
#include <vector>


class Point
{
private:
	int ID;
	std::vector<Package> availablePackages;
	std::vector<Package> originalPackages;

public:
	Point(int Id);
	Point(int Id, std::vector<Package> packages_);

	int getID();
	std::vector<Package> getAvailablePackages();
	std::vector<Package> getOriginalPackages();
	void addPackage(Package package);
	void deletePackage(Package package);
};
#endif //POINT_H
