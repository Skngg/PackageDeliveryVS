#ifndef POINT_H
#define POINT_H
#include "package.h"
#include <vector>


class Point
{
private:
	int ID;
	std::vector<Package> org_packages;
	std::vector<Package> packages;
public:
	Point(int Id);
	Point(int Id, std::vector<Package> packages_);

	int getID();
	void reset();
	std::vector<Package> getPackages();
	std::vector<Package> getOrgPackages();
	void addPackage(Package package);
	void deletePackage(Package package);
	Package take_Package(int id);
	void delete_Package_By_Id(int id);
};
#endif //POINT_H
