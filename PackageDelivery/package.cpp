#include "package.h"
int Package::number = 0;
/////////////////////////////////////////
Package::Package(int sour, int dest)
	: destination(dest), source(sour)
{
	number++;
	ID = number;
}
/////////////////////////////////////////
int Package::getID() { return ID; }
int Package::getDestination() { return destination; }
int Package::getSource() { return source; }

