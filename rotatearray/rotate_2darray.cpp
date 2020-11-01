#include <iostream>

// simple test code to rotate a 2D array (Like rotating a mino in tetris)

#define DIM 4

int array2D[DIM][DIM] =
{
    { 0, 0, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 0, 0 }
};

void printArray(int array[][DIM])
{
    for (int i = 0; i < DIM; ++i)
    {
        for(int k = 0; k < DIM; ++k)
        {
            std::cout << (array[i][k] ? 'o' : 'x');
        }
        
        std::cout << "\n";
    }
}

template<typename func>
void rotate(int res[][DIM], int array[][DIM], func f)
{
    int col = DIM - 1;
    for (int i = 0; i < DIM; ++i)
    {
        for (int k = 0; k < DIM; ++k)
        {
            f(i, k);
        }
    }
}

void rotateRight(int res[][DIM], int array[][DIM])
{
    int col = DIM - 1;
    auto lambda = [&](int i, int k) { res[k][col - i] = array[i][k]; };
    rotate(res, array, lambda);
}

void rotateLeft(int res[][DIM], int array[][DIM])
{
    int col = DIM - 1;
    auto lambda = [&](int i, int k) { res[col - k][i] = array[i][k]; };
    rotate(res, array, lambda);
}

int main()
{
    printArray(array2D);

    int res[DIM][DIM];

    for(int i = 0; i < 4; ++i)
    {
        rotateRight(res, array2D);
        std::cout << "Rotated Rightwards " << i + 1 << " time(s)\n";
        std::cout << std::endl;
        printArray(res);

        memcpy(array2D, res, sizeof(int) * DIM * DIM);
    }

    for(int i = 0; i < 4; ++i)
    {
        rotateLeft(res, array2D);
        std::cout << "Rotated Leftwards " << i + 1 << " time(s)\n";
        std::cout << std::endl;
        printArray(res);

        memcpy(array2D, res, sizeof(int) * DIM * DIM);
    }

    return 0;
}