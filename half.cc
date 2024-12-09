#include <iostream>
#include <iomanip>

int matrix[100][100] = {0};

int main(int argc, char const *argv[])
{
    int width;
    std::cout << "input the matrix's width:";
    std::cin >> width;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cin >> matrix[i][j];
            matrix[i][j] = matrix[i][j]/2.0;
        }
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout  << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
    
    return 0;
}
