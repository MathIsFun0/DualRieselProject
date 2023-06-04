// Runs the inital testing using PFGW

#include "initialTesting.hpp"
#include "common.hpp"
#include "util.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <string>
#include <fstream>
#include <future>
#include <thread>
#include <filesystem>


void initialTesting(Options options, Conjecture conjecture) {
    //Setup phase
    //Testing limit to certain # of digits, or maxN if smaller
    int initTestingLimit = std::min((double)conjecture.maxN,std::floor((double)options.initialTestingCutoff/std::log10(conjecture.base)));
    std::string sign = "-";
    if (conjecture.conjectureType == "S") {
        sign = "+";
    }
    int maxThreads = options.pfgwInstances;
    bool starting = true;
    conjecture.minN++; //JUST FOR TESTING
    for (int n = conjecture.minN; n <= initTestingLimit; n++) {
        conjecture.minN = n; //Update to reflect new minimum n untested
        if (starting) {
            starting = false;
            println("Testing small values - n < " + std::to_string(initTestingLimit));
        }
        std::vector<std::string> candidateString;
        if (conjecture.candidates.size() > 100000) { //Print warning...
            println("Creating candidate list. This may take a while...");
        }
        candidateString.reserve(conjecture.candidates.size());
        for (long candidate : conjecture.candidates) {
            //# Filter only + values for Rp
            if (conjecture.conjectureType != "Rp" || std::log(candidate) / std::log(conjecture.base) < n) {
                //# Add string representation to vector
                candidateString.push_back(std::to_string(conjecture.base)+"^"+std::to_string(n)+sign+std::to_string(candidate));
            }
        }
        candidateString.shrink_to_fit();
        int numInstances = std::min((int)candidateString.size(), options.pfgwInstances);
        std::vector<std::vector<std::string>> candidateThreadLists = splitIntoNum<std::string>(candidateString,numInstances);
        std::string basePath = "temp/"+conjecture.fileName+"-"+std::to_string(n)+"_";
        for (int i = 0; i < numInstances; i++) {
            std::string fileName = basePath + std::to_string(i)+".txt";
            //Save to file
            std::ofstream thisFile(fileName);
            thisFile << vec2string(candidateThreadLists[i], true);
            thisFile.close();
        }
        println("Testing n = "+std::to_string(n));
        std::vector<std::future<int>> futureOutcomes;
        futureOutcomes.reserve(numInstances);
        for (int i = 0; i < numInstances; i++) {
            //makes and goes to temp/pfgwX then runs it.
            //wrapped in async call so that it's, well.. async.
            //todo: make this not show output, its slow
            #ifndef _WIN32
            futureOutcomes.push_back(std::async(std::launch::async, systemWrapper, 
            "cd temp; mkdir -p pfgw"+std::to_string(i)+";cd pfgw"+std::to_string(i)+";"+
            "./../../"+options.pfgwPath+" -T"+std::to_string(options.pfgwThreadsPerInstance)+ " ../../"+basePath+std::to_string(i)+".txt"));  
            #else
            futureOutcomes.push_back(std::async(std::launch::async, systemWrapper, 
            "cd temp & if not exist \"pfgw" + std::to_string(i) + "\" mkdir pfgw"+std::to_string(i)+" & cd pfgw"+std::to_string(i)+" & "+
            "\"../../"+options.pfgwPath+"\" -T"+std::to_string(options.pfgwThreadsPerInstance)+ " \"../../"+basePath+std::to_string(i)+".txt\""));  
            #endif
        }
        for (int i = 0; i < numInstances; i++) {
            futureOutcomes[i].wait();
        }
        //Now that PFGW has finished running, process all of the output files
        //Output is in pfgw-prime.log for primes
        std::vector<int> removedKs;
        std::vector<std::string> primes;
        for (int i = 0; i < numInstances; i++) {
            //Check which ks have primes
            //Open output files
            std::ifstream primeFile("temp/pfgw"+std::to_string(i)+"/pfgw-prime.log"); // proven primes
            std::ifstream prpFile("temp/pfgw"+std::to_string(i)+"/pfgw.log"); // probable primes (>99.99% prime)
            //Parse output line by line
            std::string line;
            // Code to get inputs
            if (primeFile.is_open()) {
                bool isDone = false;
                while (!isDone) {
                    //Read the configuration file, line by line
                    if (!std::getline(primeFile, line)) {
                        isDone = true; //end of file
                    }
                    primes.push_back(line);
                    //Add current line to removedKs
                    if (conjecture.conjectureType != "S") {
                        std::vector<std::string> splitLine = split(line, '-');
                        removedKs.push_back(std::stoi(splitLine.back()));
                    } else {
                        std::vector<std::string> splitLine = split(line, '+');
                        removedKs.push_back(std::stoi(splitLine.back()));
                    }
                }
            }
            primeFile.close();
            if (prpFile.is_open()) {
                bool isDone = false;
                while (!isDone) {
                    //Read the configuration file, line by line
                    if (!std::getline(primeFile, line)) {
                        isDone = true; //end of file
                    }
                    primes.push_back(line);
                    //Add current line to removedKs
                    if (conjecture.conjectureType != "S") {
                        std::vector<std::string> splitLine = split(line, '-');
                        removedKs.push_back(std::stoi(splitLine.back()));
                    } else {
                        std::vector<std::string> splitLine = split(line, '+');
                        removedKs.push_back(std::stoi(splitLine.back()));
                    }
                }
            }
            prpFile.close();
        }
        //Remove specified Ks from candidates
        for (const int& k : removedKs) {
            conjecture.candidates.erase(std::remove(conjecture.candidates.begin(), conjecture.candidates.end(), k),
                                        conjecture.candidates.end());
        }
        
        //Add found primes to file

        // Check if the file exists
        if (!std::filesystem::exists(conjecture.fileName+"_primes.txt")) {
            std::ofstream createFile(conjecture.fileName+"_primes.txt");
            createFile.close();
        }

        std::ofstream outputFile(conjecture.fileName+"_primes.txt", std::ios::app);
        for (const auto& prime : primes) {
            outputFile << prime << "\n";
        }
        outputFile.close();

        //Cleanup
        conjecture.saveToFile();
        clearTempDirectory();
        exit(1);
    }
}