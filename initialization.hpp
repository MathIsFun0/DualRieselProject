#ifndef INITIALIZATION_HPP
#define INITIALIZATION_HPP

#include "common.hpp"
#include <vector>

//Contains all initialization and initialization-related functions
//Does NOT contain initial testing
void initialization(Options options);
void getTrivialFactors(Options options, Conjecture conjecture);
void askForFactors(Options options, Conjecture conjecture, std::vector<std::vector<int>> trivialFactors, bool firstTime = false);
void parseFilters(Options options, Conjecture conjecture, std::vector<std::vector<int>> trivialFactors);

#endif