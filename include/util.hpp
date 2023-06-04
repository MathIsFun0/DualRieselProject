#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

//These functions are explained in util.cpp.
std::vector<std::string> split(std::string string, char delimiter);
std::vector<int> primeFactors(int n);
std::vector<int> distinctPrimeFactors(int n);
std::vector<int> perfectPowers(int n, int p = 2);
long long calculateModBetween(long long x, long long y, long long m, long long n);
int systemWrapper(std::string arg);
void clearTempDirectory();
void removeDirectoryContents(const std::filesystem::path& directoryPath);

//Converts a vector of elements to a string.
std::string vec2string(std::vector<std::string> vec, bool raw = false);
template <typename T>
std::string vec2string(std::vector<T> vec, bool raw = false) {
    // raw = true: Stores each element in individual lines.
    // raw = false: Stores the vector in a readable format: [1,2,3].
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

//Splits a vector of elements into num equally-sized vectors. 
template <typename T>
std::vector<std::vector<T>> splitIntoNum(std::vector<T> vec, int num) {
    std::vector<std::vector<T>> splitList;
    //Initialize all the vectors inside splitList
    for (int i = 0; i < num; i++) {
        std::vector<T> tempVec;
        splitList.push_back(tempVec);
        splitList[i].reserve(vec.size()/num + 1);
    }
    int i = 0;
    //Add all the elements to the vectors, distributing them equally.
    for (T element : vec) {
        splitList[i].push_back(element);
        i++;
        if (i >= num) {
            i = 0;
        }
    }
    //Remove all the extra allotted space.
    for (int i = 0; i < num; i++) {
        splitList[i].shrink_to_fit();
    }
    return splitList;
}

//Functions for print and println, since I don't like using cout for this.
template <typename T>
void print(T thing) {
    std::cout << thing << std::flush;
}

template <typename T>
void println(T thing) {
    std::cout << thing << std::endl;
}
#endif