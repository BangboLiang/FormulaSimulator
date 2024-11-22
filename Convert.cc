//Convert traffic matrix to job list.

#include <iostream>
#include <iomanip>

int main(int argc, char const *argv[])
{
    int width;
    std::cout << "width:";
    std::cin >> width;
    double matrix[width][width];
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cin >> matrix[i][j];
        }
        
    }
    //stage 1
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (matrix[i][j])
            {
                std::cout << "1 " << i+1 << ' '<< j+1 << ' '<< std::fixed << std::setprecision(10) << matrix[i][j] << '\n';
            }
        }
    }
    std::cout << -1 << '\n';
    return 0;
}
