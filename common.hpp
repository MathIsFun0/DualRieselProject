#ifndef COMMON_HPP
#define COMMON_HPP

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

//Common contains Conjecture and Settings classes.
class Conjecture {
    public:
        std::string conjectureType;
        int base;
        long minK;
        long maxK;
        int startingN;
        int minN;
        int maxN;
        int step;
        std::vector<long> candidates;
        std::string state;
        std::string fileName;
        Conjecture();
        Conjecture(std::string inputFile);
        std::string print();
        void saveToFile(std::string filePath);
        void saveToFile();
};

class Options {
    public:
        bool dualRieselMode;
        int pfgwInstances;
        int pfgwThreadsPerInstance;
        int sieveInstances;
        int llrInstances;
        int llrThreadsPerInstance;
        int initialTestingCutoff;
        std::string pfgwPath;
        std::string sievePath;
        std::string llrPath;
        int stepSize;
        double sieveFactor;
        Options(std::string inputFile);
};

#endif