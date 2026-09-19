#include <iostream>
#include <limits>
#include "matrix_ops.h"

int main() 
{
    int** matrix = nullptr;
    std::size_t rows = 0;
    std::size_t cols = 0;
    int choice = -1;
    
    while (true) 
    {
        std::cout << "\nVariant 15\n";
        std::cout << "1. Create matrix\n";
        std::cout << "2. Fill with value\n";
        std::cout << "3. Print matrix\n";
        std::cout << "4. Transpose\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        
        if (!(std::cin >> choice))
         {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input error! Please enter a number.\n";
            continue;
        }
        
        switch (choice)
         {
            case 1: 
            {
                int r = 0, c = 0;
                std::cout << "rows: ";
                std::cin >> r;
                std::cout << "cols: ";
                std::cin >> c;
                if (r <= 0 || c <= 0)
                {
                    std::cout<<"Minim value rows or cols is 1\n";
                    break;
                }
                matrix_delete(matrix, rows);
                size_t rows(r);
                size_t cols(c);
                matrix = matrix_create(rows, cols);
                std::cout << "Matrix " << rows << "x" << cols << " created.\n";
                break;
            }
            case 2: 
            {
                if (!matrix) 
                {
                    std::cout << "Create matrix first!\n";
                    break;
                }
                int value;
                std::cout << "value: ";
                std::cin >> value;
                matrix_fill(matrix, rows, cols, value);
                std::cout << "Matrix filled.\n";
                break;
            }
            case 3: 
            {
                if (!matrix) 
                {
                    std::cout << "Matrix is empty.\n";
                    break;
                }
                std::cout << "Current matrix:\n";
                matrix_print(matrix, rows, cols);
                break;
            }
            case 4: 
            {
                if (!matrix) 
                {
                    std::cout << "Create matrix first\n";
                    break;
                }
                std::cout << "Original matrix:\n";
                matrix_print(matrix, rows, cols);
                std::cout << "\nTransposed matrix:\n";
                int** transposed = matrix_transpose(matrix, rows, cols);
                matrix_print(transposed, cols, rows);
                matrix_delete(transposed, cols);
                break;
            }
            case 0: 
            {
                matrix_delete(matrix, rows);
                std::cout << "Exit.\n";
                return 0;
            }
            default:
                std::cout << "Invalid option!\n";
        }
    }
    return 0;
}