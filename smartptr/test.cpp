#include <iostream>
#include "smartptr.h"

class A
{
    int _a;

public:
    A(int i) : _a(i) {}
};

class B : public A
{
public:
    B(int i) : A(i) {}
};

int main()
{
    std::cout << "a\n";
    mpaop::smartptr::MSharedPtr<uint32_t> m(nullptr);
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

    mpaop::smartptr::MSharedPtr<A> a(mpaop::smartptr::MSharedPtr<A>::create(1));

    mpaop::smartptr::MSharedPtr<B> b(mpaop::smartptr::MSharedPtr<B>::create(2));

    mpaop::smartptr::MSharedPtr<A> a2(b);

    return 0;
}