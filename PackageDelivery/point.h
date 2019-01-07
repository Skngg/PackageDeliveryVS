#ifndef POINT_H
#define POINT_H
#include "package.h"
#include <vector>


class Point
{
private:
	int ID;
	const std::vector<Package> org_packages;
	std::vector<Package> packages;
public:
	Point(int Id);
	Point(int Id, std::vector<Package> packages_);

	int getID()const;
	void reset();
	std::vector<Package> getPackages();
	std::vector<Package> getOrgPackages()const;
	void addPackage(Package package);
	void add_Package_By_Id(int id);
	void deletePackage(Package& package);
	void delete_Package_By_Id(int id);

};
#endif //POINT_H
