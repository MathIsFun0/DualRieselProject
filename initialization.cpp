#include "initialization.hpp"
#include "common.hpp"
#include "util.hpp"

#define PLUSMINUS "±"
#ifdef _WIN32
#undef PLUSMINUS
#define PLUSMINUS "+/-"
#endif

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef CLIMITS
#define CLIMITS
#include <climits>
#endif

#ifndef CSTDIO
#define CSTDIO
#include <cstdio>
#endif

#ifndef CMATH
#define CMATH
#include <cmath>
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

    print("Please enter the BASE of this conjecture - B^n ");
    print(PLUSMINUS);
    println(" k");
    std::cin >> temp;
    conjecture.base = std::stoi(temp);
    temp = "";
    if (conjecture.base < 2) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    print("Please enter the MINIMUM CANDIDATE of this conjecture - b^n ");
    print(PLUSMINUS);
    println(" K");
    std::cin >> temp;
    conjecture.minK = std::stoll(temp);
    temp = "";
    if (conjecture.minK < 0) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    print("Please enter the MAXIMUM CANDIDATE of this conjecture - b^n ");
    print(PLUSMINUS);
    println(" K");
    println("If there is no maximum candidate, use the conjectured k minus 1.");
    std::cin >> temp;
    conjecture.maxK = std::stoll(temp);
    temp = "";
    if (conjecture.maxK < conjecture.minK) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    print("Please enter the MINIMUM EXPONENT of this conjecture - b^N ");
    print(PLUSMINUS);
    println(" k");
    println("To start from scratch, use 1.");
    std::cin >> temp;
    conjecture.minN = std::stoi(temp);
    temp = "";
    if (conjecture.minN < 1) {
        println("Invalid response, please try again.");
        initialization(options);
        return;
    }

    print("Please enter the MAXIMUM EXPONENT of this conjecture - b^N ");
    print(PLUSMINUS);
    println(" k");
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
        case 'Y': getTrivialFactors(options, conjecture); break;
        case 'y': getTrivialFactors(options, conjecture); break;
        default: initialization(options);
    }
}

void getTrivialFactors(Options options, Conjecture conjecture) {
    std::vector<std::vector<int>> trivialFactors;
    //Non-algebraic Factors (type 0)
    for (int i : distinctPrimeFactors(conjecture.base)) {
        trivialFactors.push_back({0, 0, i});
    }
    for (int i : distinctPrimeFactors(conjecture.base - 1)) {
        if (conjecture.conjectureType == "S") {
            trivialFactors.push_back({0, i-1, i});
        } else {
            trivialFactors.push_back({0, 1, i});
        }
    }
    bool ignoreSpecialSquares = false;
    //Perfect Powers (type 1)
    for (int p : perfectPowers(conjecture.base)) {
        std::vector<int> prf = primeFactors(p);
        if ((conjecture.conjectureType != "S") || (std::count(prf.begin(), prf.end(), 2) != prf.size())) {//powers of 2^x don't factorize in Sierpinski
            trivialFactors.push_back({1, p});
            if (p == 2) {
                ignoreSpecialSquares = true;
            }
        }
    }
    //Special Squares (type 2)
    if (conjecture.conjectureType != "S" && !ignoreSpecialSquares) {
        for (int p : distinctPrimeFactors(conjecture.base+1)) {
            if (p % 4 == 1) {
                //Finding the modulo brute force is enough here
                int num = 0;
                int i = 1;
                while (num == 0) {
                    if ((i*i)%p == p-1) {
                        num = i;
                    }
                    i++;
                }
                trivialFactors.push_back({2, num, p});
            }
        }
    }

    askForFactors(options, conjecture, trivialFactors, true);
}

void askForFactors(Options options, Conjecture conjecture, std::vector<std::vector<int>> trivialFactors, bool firstTime) {
    if (firstTime) {
        print("Trivial/algebraic factors auto-generated. ");
    } else {
        println("Enter a factor, using the format on https://docs.google.com/document/d/1fkCgsESiV2CuZ48Vh3TTh35bOEq21gsc657teOQViWI/edit.");
        std::string response;
        std::cin >> response;
        std::vector<std::string> arrayResponse = split(response, ',');
        std::vector<int> factor;
        for (std::string arg : arrayResponse) {
            factor.push_back(stoi(arg));
        }
        trivialFactors.push_back(factor);
    }
    println("Is this information correct?");
    for (std::vector<int> factor : trivialFactors) {
        switch (factor[0]) {
            case 0: std::printf("k = %i mod %i\n", factor[1], factor[2]); break;
            case 1: std::printf("k = m^%i\n", factor[1]); break;
            case 2: std::printf("k = m^2, m = %i, %i mod %i\n", factor[1], factor[2] - factor[1], factor[2]); break;
            case 3: std::printf("k = %im^2, m = %i, %i mod %i\n", factor[1], factor[2], factor[3] - factor[2], factor[3]); break;
            case 4: std::printf("k = %i\n", factor[1]); break;
        }
    }
    println("Please enter Y to continue, N to add factors, or C to clear all factors and add factors.");
    char response;
    std::cin >> response;
    switch (response) {
        case 'Y': parseFilters(options, conjecture, trivialFactors); break;
        case 'y': parseFilters(options, conjecture, trivialFactors); break;
        case 'C': {std::vector<std::vector<int>> v; askForFactors(options, conjecture, v); break;}
        case 'c': {std::vector<std::vector<int>> v; askForFactors(options, conjecture, v); break;}
        default: askForFactors(options, conjecture, trivialFactors); break;
    }
}

void parseFilters(Options options, Conjecture conjecture, std::vector<std::vector<int>> trivialFactors) {
    //Estimate how many k will be remaining
    long long estimatedMax = conjecture.maxK - conjecture.minK + 1;
    long long estimatedMin = conjecture.maxK - conjecture.minK + 1;
    for (std::vector<int> factor : trivialFactors) {
        switch (factor[0]) {
            case 0: {
                estimatedMax = estimatedMax * (factor[2] - 1) / factor[2] + 1;
                estimatedMin = estimatedMin * (factor[2] - 1) / factor[2];
                break;
            }
            case 1: {
                estimatedMin = estimatedMin - std::floor(std::pow(conjecture.maxK, 1.0/factor[1])) + std::ceil(std::pow(conjecture.minK, 1.0/factor[1])) - 1;
                break;
            }
            case 2: {
                long long maxM = std::floor(std::sqrt(conjecture.maxK));
                long long minM = std::ceil(std::sqrt(conjecture.minK));
                estimatedMin = estimatedMin - calculateModBetween(minM, maxM, factor[1], factor[2]) - calculateModBetween(minM, maxM, factor[2] - factor[1], factor[2]);
                break;
            }
            case 3: {
                long long maxM = std::floor(std::sqrt(conjecture.maxK/factor[1]));
                long long minM = std::ceil(std::sqrt(conjecture.minK/factor[1]));
                estimatedMin = estimatedMin - calculateModBetween(minM, maxM, factor[2], factor[3]) - calculateModBetween(minM, maxM, factor[3] - factor[2], factor[3]);
                break;
            }
            case 4: {
                estimatedMax--;
                estimatedMin--;
                break;
            }
        }
    }
    println("An estimated "+std::to_string(estimatedMin)+" - "+std::to_string(estimatedMax)+" candidates will remain after filtering.");
    if (estimatedMax > 10000000) {
        println("Warning - this is a large list of candidates, and it is recommended to split up the list into multiple ranges. If you want to quit now, press Q.");
        char response;
        std::cin >> response;
        if (response == 'q' || response == 'Q') {
            exit(0);
        }
    }
    conjecture.candidates.reserve(estimatedMax);
    println("Generating candidates...");
    for (long long i = conjecture.minK; i <= conjecture.maxK; i++) {
        if (i % 1000000 == 0 || i == conjecture.maxK) {
            println("k = "+std::to_string(i));
        }
        for (std::vector<int> factor : trivialFactors) {
            switch (factor[0]) {
                case 0: {
                    if (i % factor[2] == factor[1]) {
                        goto FILTERED;
                    }
                    break;
                }
                case 1: {
                    long long nthRoot = std::floor(std::pow(i, 1.0/factor[1]));
                    if (std::pow(nthRoot, factor[1]) == i) {
                        goto FILTERED;
                    }
                    break;
                }
                case 2: {
                    long long squareRoot = std::floor(std::sqrt(i));
                    if (squareRoot * squareRoot == i) {
                        long long rootMod = squareRoot % factor[2];
                        if (rootMod == factor[1] || rootMod == factor[2] - factor[1]) {  
                            goto FILTERED;
                        }
                    }
                    break;
                }
                case 3: {
                    long long scaledI = i/factor[1];
                    long long squareRoot = std::floor(std::sqrt(scaledI));
                    if (squareRoot * squareRoot == scaledI) {
                        long long rootMod = squareRoot % factor[3];
                        if (rootMod == factor[2] || rootMod == factor[3] - factor[2]) {  
                            goto FILTERED;
                        }
                    }
                    break;
                }
                case 4: {
                    if (i == factor[1]) {
                        goto FILTERED;
                    }
                    break;
                }
            }
        }
        conjecture.candidates.push_back(i);
        FILTERED: continue;
    }
    conjecture.candidates.shrink_to_fit();
    println("There are "+std::to_string(conjecture.candidates.size())+" candidates remaining after filtering.");
}