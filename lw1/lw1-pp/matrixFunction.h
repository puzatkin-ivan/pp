#pragma once
#include <fstream>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using Matrix = std::vector<std::vector<float>>;

namespace
{

    Matrix mainMatrix;

    int matrixSize;

    int threadNumber;

    static const double EPS = 1E-9;

    struct ThreadArguments
    {
        std::istream* data;
        Matrix* matrix;
        int start, end, row, col, size;
    };

}

void MatrixOperations(std::istream& input, int threadCount);
int Rank();
int tryStringToInt(const std::string& str);
void ProcessRow(ThreadArguments* threadArguments, size_t j, size_t i, size_t d);
