#include "pch.h"
#include <ctime>
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

int main(int argc, char* argv[])
{
    HANDLE process = GetCurrentProcess();
    SetProcessAffinityMask(process, 0b100);

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

int tryStringToInt(const std::string& str)
{
    int result = 0;
    try
    {
        result = std::stoi(str, nullptr, 10);
    }
    catch (std::invalid_argument ex)
    {
        result = -1;
    }
    return result;
}

DWORD WINAPI GetMatrix(PVOID pvParam)
{
    auto threadArguments = (ThreadArguments*)pvParam;
    float number;
    std::vector<float> line;
    for (int i = 0; i < threadArguments->size; i++)
    {
        line.clear();
        for (int j = 0; j < threadArguments->size; j++)
        {
            (*(threadArguments->data)) >> number;
            line.push_back(number);
        }

        (*(threadArguments->matrix)).push_back(line);
    }

    ExitThread(0);
}

DWORD WINAPI WorkMatrix(PVOID pvParam)
{
    auto threadArguments = static_cast<ThreadArguments*>(pvParam);
    int j = threadArguments->row;
    int i = threadArguments->col;

    for (int k = threadArguments->start; k < threadArguments->end; ++k)
    {
        if (k != j && abs((*threadArguments->matrix)[k][i]) > EPS)
        {
            for (size_t p = i + 1; p < (*(threadArguments->matrix)).size(); ++p)
            {
                (*(threadArguments->matrix))[k][p] -= (*(threadArguments->matrix))[j][p] * (*(threadArguments->matrix))[k][i];
            }
        }
    }

    ExitThread(0);
}

void MatrixOperations(std::istream& input, int threads)
{
    input >> matrixSize;
    threadNumber = threads > matrixSize ? matrixSize : threads;
    auto* threadArguments = new ThreadArguments;
    threadArguments->data = &input;
    threadArguments->matrix = &mainMatrix;
    threadArguments->size = matrixSize;
    HANDLE* hTread = new HANDLE;

    *hTread = CreateThread(NULL, 0, &GetMatrix, (PVOID)threadArguments, 0, NULL);
    WaitForMultipleObjects(1, hTread, true, INFINITE);
}

int Rank()
{
    Matrix matrix(mainMatrix);
    int rank = matrixSize;
    std::vector<char> str(matrixSize);

    for (int i = 0; i < matrixSize; i++)
    {
        int j;
        for (j = 0; j < matrixSize; j++)
        {
            if ((abs(matrix[j][i]) > EPS) && !str[j]) { break; }
        }

        if (j == matrixSize)
        {
            --rank;
        }
        else
        {
            str[j] = true;
            for (int p = i + 1; p < matrixSize; p++)
            {
                matrix[j][p] /= matrix[j][i];
            }

            int step = matrixSize / threadNumber;
            int threadCounter = threadNumber;
            HANDLE* hTread = new HANDLE[threadNumber];

            for (int k = 0; threadCounter != 0; k += step)
            {
                --threadCounter;
                auto* threadArguments = new ThreadArguments;
                threadArguments->matrix = &matrix;
                threadArguments->col = i;
                threadArguments->row = j;
                threadArguments->start = k;

                threadArguments->end = (threadCounter != 0) ? k + step : matrixSize;

                hTread[threadCounter] = CreateThread(NULL, 0, &WorkMatrix, (PVOID)threadArguments, 0, NULL);
            }

            WaitForMultipleObjects(threadNumber, hTread, true, INFINITE);
        }
    }
    return rank;
}