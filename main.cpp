#include <iostream>
#include "reflist/linkedtree.h"

template<typename T>
void printChildren(mpaop::linkedtree::MLinkedLeaf<T> * leaf)
{
    std::cout << "Leaf: " << ** leaf << "\n";

    if(leaf->Children().Size() != 0)
    {
        for(auto & child : leaf->Children())
        {
            printChildren(& child);
        }
    }
}

int main()
{
    mpaop::linkedtree::MLinkedTree<int> tree;
    auto * node = tree.Push(1);
    node->pushChild(11);
    auto * node2 = node->pushChild(12);
    node2->pushChild(120);

    tree.Push(13);

    for(auto & n : tree)
    {
        printChildren(& n);
    }

    return 0;
}