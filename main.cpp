#include "util.hpp"
#include "common.hpp"
#include "initialization.hpp"
#include "initialTesting.hpp"

void readFile() {
    println("Enter the file path.");
    std::string filePath;
    std::cin >> filePath;
    Conjecture conjecture(filePath);
    conjecture.fileName = filePath+"-backup"; //prevent corruption at all costs
    conjecture.saveToFile();
    conjecture.fileName = filePath;
    if (conjecture.state == "initialTesting") { /*initialTesting(conjecture)*/ }
    else if (conjecture.state == "sieving") { /*sieveAndTest(conjecture)*/ }
    else if (conjecture.state == "testing") { /*sieveAndTest(conjecture)*/ }
    else { /*initialTesting(conjecture)*/ };
}

int main() {
    Options options("dsrs.ini");
    println("What do you want to run today?\n");
    println("[0] Continue from a save.");
    println("[1] Run a test from scratch.");
    char response;
    std::cin >> response;
    switch (response) {
        case '0':
            readFile();
            break;
        case '1':
            initialization(options);
            break;
        default:
            print("Invalid response, quitting...");
            break;
    }
}