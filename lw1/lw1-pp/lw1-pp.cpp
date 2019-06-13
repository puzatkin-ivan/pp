#include "pch.h"
#include <ctime>
#include <fstream>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "matrixFunction.h"

int main(int argc, char* argv[])
{
    HANDLE process = GetCurrentProcess();
    SetProcessAffinityMask(process, 0b01);

    clock_t start = clock();
    if (argc != 3)
    {
        std::cout << "Wrong number of arguments." << std::endl;
        std::cout << "Usage: lw1-pp.exe <input file> <tread number>" << std::endl;
        return 1;
    }

    const std::string inputFileName = argv[1];
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open())
    {
        std::cout << "Input file cannot be opened." << std::endl;
        return 1;
    }

    int threadNumber = tryStringToInt(argv[2]);
    if (threadNumber == -1 || ((threadNumber < 1) || (threadNumber > 16)))
    {
        std::cout << "Input is not a number." << std::endl;
        return 1;
    }

    MatrixOperations(inputFile, threadNumber);
    std::cout << Rank() << std::endl;

    clock_t end = clock();
    clock_t clockTicksTaken = end - start;
    double timeInMilliSeconds = clockTicksTaken;
    std::cout << "Time: " << timeInMilliSeconds << "ms";
    return 0;
}

