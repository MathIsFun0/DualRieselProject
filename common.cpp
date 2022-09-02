#include "common.hpp"
#include "util.hpp"

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef CLIMITS
#define CLIMITS
#include <climits>
#endif

Conjecture::Conjecture() {
    //Default values
    conjectureType = "";
    base = 0;
    minK = 0;
    maxK = LLONG_MAX;
    startingN = 0;
    minN = 0;
    maxN = INT_MAX;
    step = 0;
    candidates = std::vector<long long>();
    state = "";
}
Conjecture::Conjecture(std::string inputFile) {
    //Default values
    conjectureType = "";
    base = 0;
    minK = 0;
    maxK = LLONG_MAX;
    startingN = 0;
    minN = 0;
    maxN = INT_MAX;
    step = 0;
    candidates = std::vector<long long>();
    state = "";

    std::string input;
    std::string inputLine;
    std::ifstream in(inputFile);
    if (in.is_open()) {
        bool isDone = false;
        while (!isDone) {
            if (!std::getline(in, inputLine)) {
                isDone = true;
            }
            if (inputLine.find("#") != std::string::npos) {
                isDone = true;
            } else {
                input += inputLine;
                input += "\n";
            }
        }
        in.close();
    } else {
        std::cout << "Unable to open file\n"; //Can't do print() because it assumes Conjecture::print()
        exit(1);
    }
    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end()); //In case Linux reads a Windows file - https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
    std::vector<std::string> splitInput = split(input, '\n');
    for (std::string arg : splitInput) {
        if (arg[0] == '#') {
            continue;
        };
        std::vector<std::string> splitArg = split(arg, '=');
        //Here, stoi and stoll convert string to other types
        if (splitArg[0] == "conjectureType") {conjectureType = splitArg[1];}
        else if (splitArg[0] == "state") {state = splitArg[1];}
        else if (splitArg[0] == "base") {base = std::stoi(splitArg[1]);}
        else if (splitArg[0] == "minK") {minK = std::stoll(splitArg[1]);}
        else if (splitArg[0] == "maxK") {maxK = std::stoll(splitArg[1]);}
        else if (splitArg[0] == "minN") {minN = std::stoi(splitArg[1]);}
        else if (splitArg[0] == "maxN") {maxN = std::stoi(splitArg[1]);}
        else if (splitArg[0] == "step") {step = std::stoi(splitArg[1]);}
        else if (splitArg[0] == "candidates") {
            splitArg[1].erase(std::remove(splitArg[1].begin(), splitArg[1].end(), '['), splitArg[1].end());
            splitArg[1].erase(std::remove(splitArg[1].begin(), splitArg[1].end(), ']'), splitArg[1].end());
            std::vector<std::string> splitCandidates = split(splitArg[1], ',');
            for (std::string candidate : splitCandidates) {
                candidates.push_back(std::stoll(candidate));
            }
        }
    }
    startingN = minN;
    fileName = conjectureType + std::to_string(base) + "-" + std::to_string(minK) + "-" + std::to_string(maxK) + "-" + std::to_string(minN) + ".ini";
}

std::string Conjecture::print() {
    return "conjectureType=" + conjectureType + "\n" +
           "state=" + state + "\n" +
           "base=" + std::to_string(base) + "\n" +
           "minK=" + std::to_string(minK) + "\n" +
           "maxK=" + std::to_string(maxK) + "\n" +
           "minN=" + std::to_string(minN) + "\n" +
           "maxN=" + std::to_string(maxN) + "\n" +
           "step=" + std::to_string(step) + "\n" +
           "candidates=" + vec2string<long long>(candidates) + "\n";
}

void Conjecture::saveToFile(std::string filePath) {
    std::ofstream file(filePath);
    file << print() << std::endl;
    file.close();
}

void Conjecture::saveToFile() {
    std::ofstream file(fileName);
    file << print() << std::endl;
    file.close();
}

Options::Options(std::string inputFile) {
    //Default values
    dualRieselMode = false;
    pfgwInstances = 1;
    pfgwThreadsPerInstance = 1;
    sieveInstances = 1;
    llrInstances = 1;
    llrThreadsPerInstance = 1;
    initialTestingCutoff = 5000;
    pfgwPath = "pfgw64.exe";
    sievePath = "sr2sieve.exe";
    llrPath = "llr2.exe";
    stepSize = 10000;
    sieveFactor = 1;

    std::string input;
    std::string inputLine;
    std::ifstream in(inputFile);
    if (in.is_open()) {
        bool isDone = false;
        while (!isDone) {
            if (!std::getline(in, inputLine)) {
                isDone = true;
                continue;
            }
            if (inputLine.find("#") != std::string::npos) {
                continue;
            } else {
                input += inputLine;
                input += "\n";
            }
        }
        in.close();
    } else {
        println("Unable to open file");
        exit(1);
    }
    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end()); //In case Linux reads a Windows file - https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
    std::vector<std::string> splitInput = split(input, '\n');
    for (std::string arg : splitInput) {
        if (arg[0] == '#') {
            continue;
        };
        std::vector<std::string> splitArg = split(arg, '=');
        //Here, stoi and stoll convert string to other types
        if (splitArg[0] == "dualRieselMode") dualRieselMode = (splitArg[1] == "true");
        if (splitArg[0] == "pfgwInstances") pfgwInstances = std::stoi(splitArg[1]);
        if (splitArg[0] == "pfgwThreadsPerInstance") pfgwThreadsPerInstance = std::stoi(splitArg[1]);
        if (splitArg[0] == "sieveInstances") sieveInstances = std::stoi(splitArg[1]);
        if (splitArg[0] == "llrInstances") llrInstances = std::stoi(splitArg[1]);
        if (splitArg[0] == "llrThreadsPerInstance") llrThreadsPerInstance = std::stoi(splitArg[1]);
        if (splitArg[0] == "initialTestingCutoff") initialTestingCutoff = std::stoi(splitArg[1]);
        if (splitArg[0] == "pfgwPath") pfgwPath = splitArg[1];
        if (splitArg[0] == "sievePath") sievePath = splitArg[1];
        if (splitArg[0] == "llrPath") llrPath = splitArg[1];
        if (splitArg[0] == "stepSize") stepSize = std::stoi(splitArg[1]);
        if (splitArg[0] == "sieveFactor") sieveFactor = std::stoi(splitArg[1]);
    }
}