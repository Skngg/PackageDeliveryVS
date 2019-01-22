#ifndef INIT_H
#define INIT_H
#include "point.h"
#include "package.h"
#include <fstream>
#include <string>
#include <sstream>
#include <random>

void initFromFile(std::vector<Package>& _packs);
std::vector<std::vector<int>> costMatrixReadCSV();
void generatePackages(int maxP, int nodes);

#endif