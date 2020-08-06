#include "uoleaf.h"
#include <iostream>

auto main() -> int
{
    mpaop::smartptr::uoleaf::MUnorderedLeaf<int> leaf(0, 10);
    mpaop::smartptr::uoleaf::MUnorderedLeaf<int> leaf2 = leaf.insert<int>(1, 2);

    std::cout << * leaf2 << "\n";

    return 0;
}