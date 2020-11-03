#include <iostream>

// simple test code to rotate a 1D array (Like rotating a mino in tetris)

#define SIZE 16
#define DIM 4

int array1D[SIZE] =
{
    0, 0, 0, 0,
    1, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
};

enum eRot
{
    r0,
    r90,
    r180,
    r270
};

void printArray(int * array)
{
    for (int i = 0; i < SIZE; ++i)
    {
        if ((i % DIM) == 0) std::cout << "\n";
        std::cout << (array[i] ? 'o' : 'x');
    }
}

int getRotatedIndex(const int & x, const int & y, const eRot & r)
{
    switch(r)
    {
        case r0:
            return x + (y * DIM);
        case r90:
            return (DIM * (DIM - 1)) - (x * DIM) + y;
        case r180:
            return ((DIM * DIM) - 1) - x - (y * DIM);
        case r270:
            return (DIM - 1) + (x * DIM) - y;    
    }

    return 0;
}

void rotate(int * res, int * array, const eRot & r)
{
    for (int i = 0; i < SIZE; ++i)
    {
        int x = i % DIM;
        int y = i / DIM;
        res[i] = array[getRotatedIndex(x, y, r)];
    }
}

int main()
{
    std::cout << "Printing array before rotation";

    printArray(array1D);

    for (int i = 0; i < 4; ++i)
    {
        int res[SIZE];
        std::cout << "\nRotated array " << (i) << " time(s)\n";
        rotate(res, array1D, static_cast<eRot>(i));
        printArray(res);
    }

    return 0;
}