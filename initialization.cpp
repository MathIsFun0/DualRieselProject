#include "initialization.hpp"
#include "common.hpp"
#include "util.hpp"

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef CLIMITS
#define CLIMITS
#include <climits>
#endif

void initialization(Options options) {
    Conjecture conjecture;
    if (options.dualRieselMode) {
        conjecture.conjectureType = "Rp";
        conjecture.base = 2;
        conjecture.minK = 1;
        conjecture.maxK = 509202;
        conjecture.minN = 1;
        conjecture.maxN = INT_MAX; 
    }
    std::string temp;
    println("Which type of dual conjecture would you like to run?");
    println("S - Sierpinski (b^n+k)");
    println("Rp - Riesel (b^n-k), not including absolute value of negative terms");
    println("Ra - Riesel (b^n+k), including the absolute value of negative terms");
    std::cin >> conjecture.conjectureType;
    std::vector<std::string> possibleConjectureTypes = std::vector<std::string>{"S", "Ra", "Rp"};
    if (!std::count(possibleConjectureTypes.begin(), possibleConjectureTypes.end(), conjecture.conjectureType)) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    println("Please enter the BASE of this conjecture - B^n ± k");
    std::cin >> temp;
    conjecture.base = std::stoi(temp);
    temp = "";
    if (conjecture.base < 2) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    println("Please enter the MINIMUM CANDIDATE of this conjecture - b^n ± K");
    std::cin >> temp;
    conjecture.minK = std::stol(temp);
    temp = "";
    if (conjecture.minK < 0) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    println("Please enter the MAXIMUM CANDIDATE of this conjecture - b^n ± K");
    println("If there is no maximum candidate, use the conjectured k minus 1.");
    std::cin >> temp;
    conjecture.maxK = std::stol(temp);
    temp = "";
    if (conjecture.maxK < conjecture.minK) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    println("Please enter the MINIMUM EXPONENT of this conjecture - b^N ± k");
    println("To start from scratch, use 1.");
    std::cin >> temp;
    conjecture.minN = std::stoi(temp);
    temp = "";
    if (conjecture.minN < 1) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    println("Please enter the MAXIMUM EXPONENT of this conjecture - b^N ± k");
    println("To have no limit, use 0.");
    std::cin >> temp;
    conjecture.maxN = std::stoi(temp);
    temp = "";
    if (conjecture.maxN < conjecture.minN && conjecture.maxN != 0) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }
    if (conjecture.maxN == 0) {
        conjecture.maxN = INT_MAX; 
    }
    println("Is the following information correct? (Y/N)");
    println("Conjecture: " + conjecture.conjectureType + std::to_string(conjecture.base));
    println("Candidates: " + std::to_string(conjecture.minK) + "-" + std::to_string(conjecture.maxK));
    if (conjecture.maxN == INT_MAX) {
        println("Exponents: Starting from " + std::to_string(conjecture.minN));
    } else {
        println("Exponents: " + std::to_string(conjecture.minN) + "-" + std::to_string(conjecture.maxN));
    }
    char decision;
    std::cin >> decision;
    switch (decision) {
        case 'Y': /*getTrivialFactors(conjecture);*/ break;
        case 'y': /*getTrivialFactors(conjecture);*/ break;
        default: initialization(options);
    }
}