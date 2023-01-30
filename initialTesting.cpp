#include "initialTesting.hpp"
#include "common.hpp"
#include "util.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>

#define BOOST_THREAD_VERSION 4
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>

#ifdef _WIN32
#define EXECPREFIX ""
#else
#define EXECPREFIX "./"
#endif



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

    while (conjecture.minN <= initTestingLimit) {
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
            if (conjecture.conjectureType != "Rp" || std::log(candidate) / std::log(conjecture.base) < conjecture.minN) {
                //# Add string representation to vector
                candidateString.push_back(std::to_string(conjecture.base)+"^"+std::to_string(conjecture.minN)+sign+std::to_string(candidate));
            }
        }
        candidateString.shrink_to_fit();
        int numInstances = std::min((int)candidateString.size(), options.pfgwInstances);
        std::vector<std::vector<std::string>> candidateThreadLists = splitIntoNum<std::string>(candidateString,numInstances);
        std::string basePath = "temp/"+conjecture.fileName+"-"+std::to_string(conjecture.minN)+"_";
        for (int i = 0; i < numInstances; i++) {
            std::string fileName = basePath + std::to_string(i)+".txt";
            //Save to file
            std::ofstream thisFile(fileName);
            thisFile << vec2string(candidateThreadLists[i], true);
            thisFile.close();
        }
        println("Testing n = "+std::to_string(conjecture.minN));
        std::vector<boost::future<int>> futureOutcomes;
        futureOutcomes.reserve(numInstances);
        for (int i = 0; i < numInstances; i++) {
            //makes and goes to temp/pfgwX, then runs it.
            //wrapped in async call so that it's, well.. async.
            //todo: support windows with boost (https://stackoverflow.com/questions/45565412/invalid-use-of-incomplete-type-using-stdfuture)
            futureOutcomes.push_back(boost::async(boost::launch::async, systemWrapper, 
            "cd temp; mkdir -p pfgw"+std::to_string(i)+";cd pfgw"+std::to_string(i)+";"+
            EXECPREFIX+"../../"+options.pfgwPath+" -T"+std::to_string(options.pfgwThreadsPerInstance)+ " ../../"+basePath+std::to_string(i)+".txt"));  
        }
        for (int i = 0; i < numInstances; i++) {
            futureOutcomes[i].wait();
        }
        exit(0);
    }
}