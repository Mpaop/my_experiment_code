#include <iostream>
#include "reflist/reflist.h"

int main()
{
    mpaop::reflist::MRefList<int> list;
    list.Push(0);
    list.Push(1);
    list.Pop();
    auto * node = list.Push(2);

    mpaop::reflist::MRefList<int> list2;
    list2.PushNode(* node);
    *list2.Current() = 3;

    list.Remove(1);
    list.Insert(0, 4);

    for(auto & n : list) std::cout << *n << "\n";
    for(auto & n : list2) std::cout << *n << "\n";

    return 0;
}