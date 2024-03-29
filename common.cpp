//Common: Defines core classes used by other parts of the program.

#include "common.hpp"
#include "util.hpp"

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>

// Stores information about the Dual Riesel/Sierpinski conjecture being tested, and the limits to test up to.
// Can be saved/loaded from a .ini file
Conjecture::Conjecture() {
    //Default values
    conjectureType = "";
    base = 0;
    minK = 0;
    maxK = LLONG_MAX; //maximum possible value
    startingN = 0;
    minN = 0;
    maxN = INT_MAX; //maximum possible value
    step = 0;
    candidates = std::vector<long long>();
    state = "";
}

Conjecture::Conjecture(std::string inputFile) {
    //Default values
    conjectureType = "";
    base = 0;
    minK = 0;
    maxK = LLONG_MAX; //maximum possible value
    startingN = 0;
    minN = 0;
    maxN = INT_MAX; //maximum possible value
    step = 0;
    candidates = std::vector<long long>();
    state = "";

    std::string input; //Input will be stored here
    std::string inputLine; //The current line of the input file
    std::ifstream in(inputFile);
    
    // Code to get inputs
    if (in.is_open()) {
        bool isDone = false;
        while (!isDone) {
            //Read the configuration file, line by line
            if (!std::getline(in, inputLine)) {
                isDone = true; //end of file
            } else {
                input += inputLine;
                input += "\n";
            }
        }
        in.close();
    } else {
        std::cout << "Unable to open file\n";
        exit(1);
    }
    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end()); //Linux compatibility for newlines - https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
    std::vector<std::string> splitInput = split(input, '\n'); //Split into each line
    for (std::string arg : splitInput) {
        if (arg[0] == '#') {
            continue; // ignore comments
        };
        std::vector<std::string> splitArg = split(arg, '='); //Split optionname and value
        //Parse inputs - the functions stoi and stoll convert strings to the appropriate type of number (i or ll)
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
    fileName = split(inputFile, '.')[0];    
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
    std::ofstream file(fileName+".ini");
    file << print() << std::endl;
    file.close();
}

// Stores the configuration of DSRS.
// Can be saved/loaded from an ini file.
Options::Options(std::string inputFile) {
    //Default values
    dualRieselMode = false; // automatically runs Riesel Base 2
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

    //The method of getting inputs is the same as in the Conjecture class.
    std::string input;
    std::string inputLine;
    std::ifstream in(inputFile);
    if (in.is_open()) {
        bool isDone = false;
        while (!isDone) {
            if (!std::getline(in, inputLine)) {
                isDone = true;
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