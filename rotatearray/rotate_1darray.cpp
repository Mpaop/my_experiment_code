#include <iostream>

// simple test code to rotate a 1D array (Like rotating a mino in tetris)

#define SIZE 16
#define DIM 4

int array1D[SIZE] =
{
    0, 0, 0, 0,
    1, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0
};

void printArray(int * array)
{
    for (int i = 0; i < SIZE; ++i)
    {
        if ((i % DIM) == 0) std::cout << "\n";
        std::cout << (array[i] ? 'o' : 'x');
    }
}

template<typename func>
void rotate(int * res, int * array, func f)
{
    for (int i = 0; i < SIZE; ++i)
    {
        int x = i % DIM;
        int y = i / DIM;
        res[f(x, y)] = array[i];
    }
}

void rotateRight(int * res, int * array)
{
    auto lambda = [](const int & x, const int & y) { return (DIM - 1 - y) + x * DIM; };

    rotate(res, array, lambda);
}

void rotateLeft(int * res, int * array)
{
    auto lambda = [](const int & x, const int & y) { return (DIM - 1 - x) * DIM + y; };

    rotate(res, array, lambda);
}

int main()
{
    std::cout << "Printing array before rotation";

    printArray(array1D);

    for (int i = 0; i < 4; ++i)
    {
        int res[SIZE];
        std::cout << "\nRotated array rightwards " << (i + 1) << " time(s)\n";
        rotateRight(res, array1D);
        printArray(res);

        memcpy(array1D, res, sizeof(int) * SIZE);
    }

    for (int i = 0; i < 4; ++i)
    {
        int res[SIZE];
        std::cout << "\nRotated array leftwards " << (i + 1) << " time(s)\n";
        rotateLeft(res, array1D);
        printArray(res);

        memcpy(array1D, res, sizeof(int) * SIZE);
    }

    return 0;
}