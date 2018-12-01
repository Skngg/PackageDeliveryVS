#ifndef PACKAGE_H
#define PACKAGE_H

class Package
{
private:
	int ID;
	int destination;
	int source;
	static int number;
public:
	Package(int sour, int dest);
	int getID();
	int getDestination();
	int getSource();
};
#endif //PACKAGE_H
