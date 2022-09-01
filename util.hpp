#ifndef UTIL_HPP
#define UTIL_HPP

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

std::vector<std::string> split(std::string string, char delimiter);

std::vector<int> primeFactors(int n);
std::vector<int> distinctPrimeFactors(int n);
std::vector<int> perfectPowers(int n);
long calculateModBetween(long x, long y, long m, long n);

template <typename T>
std::string vec2string(std::vector<T> vec) {
    std::string result = "[";
    for (long unsigned i = 0; i < vec.size(); i++) {
        result += std::to_string(vec[i]);
        if (i != vec.size() - 1) {
            result += ",";
        }
    }
    result += "]";
    return result;
}

template <typename T>
void print(T thing) {
    std::cout << thing << std::flush;
}

template <typename T>
void println(T thing) {
    std::cout << thing << std::endl;
}
#endif