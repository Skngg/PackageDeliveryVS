#ifndef INIT_H
#define INIT_H
#include "point.h"
#include "package.h"
#include <fstream>
#include <string>
#include <sstream>

void initFromFile(std::vector<Point>& _points, int n);
std::vector<std::vector<int>> costMatrixReadCSV();

#endif