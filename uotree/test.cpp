#include "uoleaf.h"
#include <iostream>

auto main() -> int
{
    mpaop::smartptr::uoleaf::MUnorderedLeaf<int> leaf(0);
    auto * leaf2 = leaf.insert<int>(1, 2);

    std::cout << * leaf2->get() << "\n";

    return 0;
}