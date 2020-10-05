#include <iostream>
#include <random>
#include <cmath>
#include <limits>

auto main() -> int
{
    // get number of throws
    int throws;
    std::cout << "Number of throws: ";
    std::cin >> throws;
    std::cout << std::flush;

    // random generator
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<double> dis(0.0, std::nextafter(1.0, std::numeric_limits<double>::max()));

    // counts number of dots within circle
    int inCir = 0;

    for(int i = 0; i < throws; ++i)
    {
        double x = dis(gen);
        double y = dis(gen);

        if(std::sqrt(x * x + y * y) <= 1.0) ++inCir;
    }

    std::cout << "pi estimate = " << (4.0 * inCir / throws) << std::flush;

    return 0;
}