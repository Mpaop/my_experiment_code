// This file is soley created for the purpose of playing around with macros.
// It is "NOT" intended to suggest practical use at all.
#include <cstdio>

// Hello, World macro.
#define FOO int main() { \
            printf("Hello, World!\n"); \
            return 0; \
            }

// Macro Interface
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
    virtual void test() = 0;
};

#define INTERFACE(T) void init() override { pFunc = static_cast<Base::fSign>(& T::sameNameFunc); }; \
                     void test() override;


class Foo : public Base
{
public:
    void sameNameFunc()
    {
        printf("Foo says Hi\n");
    }

    INTERFACE(Foo)
};

void Foo::test()
{
    printf("Foo overrides test() from Base\n");
}

class Bar : public Base
{
public:
    void sameNameFunc()
    {
        printf("Bar says Hi\n");
    }

    INTERFACE(Bar)

};

void Bar::test()
{
    printf("Bar overrides test() from Base\n");
}

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
        pBase[i]->test();
    }

    return 0;
}