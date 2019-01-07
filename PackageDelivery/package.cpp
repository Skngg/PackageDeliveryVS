#include "package.h"
int Package::number = 0;
/////////////////////////////////////////
Package::Package(int sour, int dest)
	: destination(dest), source(sour)
{
	ID = number;
	number++;
}
/////////////////////////////////////////
int Package::getID()const { return ID; }
int Package::getDestination() { return destination; }
int Package::getSource() { return source; }

