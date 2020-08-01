// This file is soley created for the purpose of playing around with templates.
// It is "NOT" intended to suggest practical use at all.
#include <cstdio>

class Base
{
public:
    using fSign = void(Base::*)();
    fSign pFunc;

    void callpFunc()
    {
        if(pFunc != nullptr) (this->*pFunc)();
    }

    virtual void init() = 0;
};

// Macro Interface
template<typename T>
class TempBase : public Base
{
public:

    void init() override
    {
        pFunc = static_cast<Base::fSign>(& T::sameNameFunc);
    }
};

class Foo : public TempBase<Foo>
{
public:
    void sameNameFunc()
    {
        printf("Foo says Hi\n");
    }
};

class Bar : public TempBase<Bar>
{
public:
    void sameNameFunc()
    {
        printf("Bar says Hi\n");
    }
};

int main()
{
    Foo foo;
    Bar bar;
    Base * pBase[] { & foo, & bar };

    for(int i = 0; i < 2; ++i)
    {
        printf("///////////////////////////\n");
        pBase[i]->init();
        pBase[i]->callpFunc();
    }

    return 0;
}