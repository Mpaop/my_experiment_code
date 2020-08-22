#include "uotree.h"
#include <iostream>

auto main() -> int
{
    
    mpaop::smartptr::uotree::MUnorderedTree<int> tree(5, 10);
    for(int32_t i = 0, j = 11; i <= 10; ++i, ++j)
    {
        std::cout << "Do Insert\n";
        if(i != 5) tree.insert(i, j);
        std::cout << "Next\n";
    }

    std::cout << (** tree.find(3)) << std::endl;
    std::cout << (** tree.find(6)) << std::endl;

    tree.remove(2);

    tree.insert(12, 5);

    tree.remove(5);

    return 0;
}