#include "pch.h"
#include "matrixFunction.h"

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

DWORD WINAPI ReadMatrix(PVOID pvParam)
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
            ProcessRow(threadArguments, j, i, k);
        }
    }

    ExitThread(0);
}

void ProcessRow(ThreadArguments* threadArguments, size_t j, size_t i, size_t d)
{
    for (size_t p = i + 1; p < (*(threadArguments->matrix)).size(); ++p)
    {
        (*(threadArguments->matrix))[d][p] -= (*(threadArguments->matrix))[j][p] * (*(threadArguments->matrix))[d][i];
    }
}

void DoExecute(std::istream& input, int threads)
{
    input >> matrixSize;
    threadNumber = threads > matrixSize ? matrixSize : threads;
    auto* threadArguments = new ThreadArguments;
    threadArguments->data = &input;
    threadArguments->matrix = &mainMatrix;
    threadArguments->size = matrixSize;
    HANDLE* hTread = new HANDLE;

    *hTread = CreateThread(NULL, 0, &ReadMatrix, (PVOID)threadArguments, 0, NULL);
    WaitForMultipleObjects(1, hTread, true, INFINITE);
}

int CalculateRank()
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