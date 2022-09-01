#include "util.hpp"
#ifndef SSTREAM
#define SSTREAM
#include <sstream>
#endif

#ifndef CMATH
#define CMATH
#include <cmath>
#endif

std::vector<std::string> split(std::string string, char delimiter) {
    //https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::vector<std::string> result;
    std::stringstream ss(string);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

std::vector<int> primeFactors(int n) {
    std::vector<int> factors;
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }
    for (int i = 3; i <= std::sqrt(n); i += 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 2) {
        factors.push_back(n);
    }
    return factors;
}

std::vector<int> distinctPrimeFactors(int n) {
    std::vector<int> factors;
    if (n % 2 == 0) {
        factors.push_back(2);
        while (n % 2 == 0) {
            n /= 2;
        }
    }
    for (int i = 3; i <= std::sqrt(n); i += 2) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) {
                n /= i;
            }
        }
    }
    if (n > 2) {
        factors.push_back(n);
    }
    return factors;
}

std::vector<int> perfectPowers(int n) {
    std::vector<int> powers;
    for (int base = 2; base <= std::sqrt(n); base++) {
        int currentNum = base * base;
        int power = 2;
        while (currentNum <= n) {
            if (currentNum != n) {
                currentNum *= base;
                power++;
            } else {
                powers.push_back(power);
                break;
            }
        }
    }
    return powers;
}

long calculateModBetween(long x, long y, long m, long n) {
    //Calculates number of numbers from x-y that are m mod n.
    long startCoeff = (x + ((m - x) % n) - m)/n;
    long endCoeff = (y - ((y - m) % n) - m)/n;
    return endCoeff - startCoeff + 1;
}