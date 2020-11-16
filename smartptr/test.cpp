#include <iostream>
#include "smartptr.h"

int main()
{
    std::cout << "a\n";
    mpaop::smartptr::MSharedPtr<uint32_t> m;
        std::cout << "b\n";
    m = nullptr;

    std::cout << "c\n";
    m = mpaop::smartptr::MSharedPtr<uint32_t>::create(10);    
    std::cout << "d\n";

    mpaop::smartptr::MSharedPtr<uint32_t> n(m);

    std::cout << "e\n";

    mpaop::smartptr::MSharedPtr<uint32_t> o(mpaop::smartptr::MSharedPtr<uint32_t>::create(20));

    std::cout << "f\n";

    o = m;

    std::cout << "g\n";

    o = o;

    std::cout << "h\n";

    return 0;
}