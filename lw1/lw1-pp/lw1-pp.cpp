#include "pch.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <locale>
#include <ctime>

using MatrixRow = std::vector<double>;
using Matrix = std::vector<MatrixRow>;

namespace
{

    static const int COUNT_INPUT_ARGUMENT = 3;

    static const double EPS = 1e-9;

}

MatrixRow ReadMatrixRow(const std::string& row)
{
    std::string s = row;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    MatrixRow matrixRow;
    std::stringstream stream(s);

    while (!stream.eof())
    {
        int cell;
        stream >> cell;
        matrixRow.push_back(cell);
    }

    return matrixRow;
}

Matrix ReadMatrix(std::istream& streamIn)
{
    Matrix matrix;
    std::string row;
    while (std::getline(streamIn, row))
    {
        auto rowMatrix = ReadMatrixRow(row);
        matrix.push_back(rowMatrix);
    }
    return matrix;
}

void PrintMatrix(std::ostream& streamOut, const Matrix& matrix)
{
    for (const auto& row : matrix)
    {
        for (auto cell : row)
        {
            streamOut << cell << ' ';
        }
        streamOut << std::endl;
    }
}

MatrixRow AdditionOfLines(const MatrixRow& lhs, const MatrixRow& rhs, size_t beginIndex)
{
    double coeff = (lhs[beginIndex] != 0) ? -(rhs[beginIndex] / lhs[beginIndex]) : 0;
    std::cout << lhs[beginIndex] << std::endl;
    MatrixRow result;
    for (size_t index = 0; index < lhs.size(); ++index)
    {
        result.push_back(rhs[index] + (coeff * lhs[index]));
    }
    return result;
}

int GetCountNonzeroRow(const Matrix& matrix)
{
    int count = 0;
    for (size_t index = 0; index < matrix.size(); ++index)
    {
        bool isNonZeroRow = false;
        for (size_t j = 0; j < matrix[0].size(); ++j)
        {
            if (matrix[index][j] != 0)
            {
                isNonZeroRow = true;
                break;
            }
        }
        count += (isNonZeroRow) ? 1 : 0;
    }
    return count;
}

double GetRangMatrix(Matrix& matrix)
{
    std::cout << std::endl;
    size_t minSize = std::min(matrix.size(), matrix[0].size());
    for (size_t index = 0; index < minSize; ++index)
    {
        if (matrix[index][index] != 0)
        {
            auto coeff = matrix[index][index];
            for (size_t index2 = 0; index2 < matrix[index].size(); ++index2)
            {
                matrix[index][index2] /= coeff;
            }
            for (size_t j = index + 1; j < matrix.size(); ++j)
            {
                matrix[j] = AdditionOfLines(matrix[index], matrix[j], index);
            }
        }
    }

    double rang = GetCountNonzeroRow(matrix);

    return rang;
}

int tryStringToInt(const std::string& threadCount)
{
    int result = 0;
    try
    {
        result = std::stoi(threadCount, nullptr, 10);
    }
    catch (std::invalid_argument ex)
    {
        result = -1;
    }
    return result;
}

int main(int argc, char *argv[])
{
    if (argc != COUNT_INPUT_ARGUMENT)
    {
        std::cout << "Error: Invalid count input argument" << std::endl;
        std::cout << "Usage: lw1.exe <inputFileName>" << std::endl;
        return 1;
    }
    const std::string inputFileName = argv[1];
    std::ifstream streamIn(inputFileName);
    if (!streamIn.is_open())
    {
        std::cout << "Error: Impossible to open input file" << std::endl;
        return 1;
    }

    std::time_t begin = std::time(nullptr);
    auto matrix = ReadMatrix(streamIn);

    auto rang = GetRangMatrix(matrix);

    std::time_t end = std::time(nullptr);
    std::cout << "Rang: " << rang << std::endl;
    std::cout << "Time: " << end - begin << std::endl;
}
