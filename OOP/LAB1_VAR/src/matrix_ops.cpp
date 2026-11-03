#include "matrix_ops.h"
#include <iostream>

int** matrix_create(std::size_t rows, std::size_t cols)
{
    if (rows == 0 || cols == 0) return nullptr;
    
    int** m = new int*[rows];
    for (std::size_t i = 0; i < rows; i++)
    {
        m[i] = new int[cols]{}; // 0 инициализация 
    }

    return m;
}

void matrix_delete(int**& m, std::size_t rows)//передавать с & или в main ставить nullptr
{
    if (!m) return;

    for (std::size_t i = 0; i < rows; i++)
    {
        delete[] m[i];
    }
    delete[] m;
    m = nullptr;//ЛИБО В MAIN 
}

void matrix_fill(int** m, std::size_t rows, std::size_t cols, int value)
{
    if (!m) return;
    for (std::size_t i = 0; i < rows; i++)
    {
        for (std::size_t j = 0; j < cols; j++)
        {
            m[i][j] = value;
        }
    }
}

void matrix_print(const int* const* m, std::size_t rows, std::size_t cols)
{
    if (!m) 
    {
        std::cout << "Matrix is empty\n";
        return;
    }
    
    for (std::size_t i = 0; i < rows; i++)
    {
        for (std::size_t j = 0; j < cols; j++)
        {
            std::cout << m[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

int** matrix_transpose(const int* const* m, std::size_t rows, std::size_t cols) 
{
    if (!m) return nullptr;

    int** res = matrix_create(cols, rows);
    for (std::size_t i = 0; i < rows; i++) 
    {
        for (std::size_t j = 0; j < cols; j++) 
        {
            res[j][i] = m[i][j];
        }
    }
    return res;
}