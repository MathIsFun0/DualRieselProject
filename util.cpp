//Stores general utility functions used by multiple files.

#include "util.hpp"

#include <cmath>
#include <sstream>

// Converts a vector of strings to a string.
std::string vec2string(std::vector<std::string> vec, bool raw) {
    // raw = true: Stores each element in individual lines.
    // raw = false: Stores the vector in a readable format: [1,2,3].
    std::string result = "[";
    if (raw) result = "";
    for (long long unsigned i = 0; i < vec.size(); i++) {
        result += vec[i];
        if (i != vec.size() - 1) {
            if (raw) {
                result += "\n";
            } else {
                result += ",";
            }
        }
    }
    if (!raw) result += "]";
    return result;
}

// Splits a string based on a delimiter, returning the substrings.
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

// Finds the prime factors of an int n, returning them as a vector
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

// Finds the distinct prime factors of an int n, returning them as a vector
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

//Finds all perfect powers of n with an exponent of at least p.
//p defaults to 2.
std::vector<int> perfectPowers(int n, int p) {
    std::vector<int> powers;
    for (int base = 2; base <= std::pow(n, 1.0/p); base++) {
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
//Calculates number of numbers between x and y inclusive that are m mod n.
long long calculateModBetween(long long x, long long y, long long m, long long n) {
    long long startCoeff = (x + ((m - x) % n) - m)/n;
    long long endCoeff = (y - ((y - m) % n) - m)/n;
    return endCoeff - startCoeff + 1;
}

//Runs a command in the command line using std::system.
int systemWrapper(std::string arg) {
    return std::system(arg.c_str());
}