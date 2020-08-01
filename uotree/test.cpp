#include "uoleaf.h"

auto main() -> int
{
    mpaop::MUnorderedLeaf<int> leaf(0);
    leaf.insert(1, 2);
    return 0;
}