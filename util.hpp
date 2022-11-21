#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string string, char delimiter);
std::vector<int> primeFactors(int n);
std::vector<int> distinctPrimeFactors(int n);
std::vector<int> perfectPowers(int n, int p = 2);
long long calculateModBetween(long long x, long long y, long long m, long long n);
int systemWrapper(std::string arg);

std::string vec2string(std::vector<std::string> vec, bool raw = false);
template <typename T>
std::string vec2string(std::vector<T> vec, bool raw = false) {
    std::string result = "[";
    if (raw) result = "";
    for (long long unsigned i = 0; i < vec.size(); i++) {
        result += std::to_string(vec[i]);
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

template <typename T>
std::vector<std::vector<T>> splitIntoNum(std::vector<T> vec, int num) {
    std::vector<std::vector<T>> splitList;
    for (int i = 0; i < num; i++) {
        std::vector<T> tempVec;
        splitList.push_back(tempVec);
        splitList[i].reserve(vec.size()/num + 1);
    }
    int i = 0;
    for (T element : vec) {
        splitList[i].push_back(element);
        i++;
        if (i >= num) {
            i = 0;
        }
    }
    for (int i = 0; i < num; i++) {
        splitList[i].shrink_to_fit();
    }
    return splitList;
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