#ifndef DEBUG_ALGO
#define DEBUG_ALGO

#include <iostream>
#include <cstdio>
#include <string>

namespace mdebug
{
    template<class ...Args>
    void logMessage(const char * format, Args... args)
    {
#ifdef MDEBUG_MODE
        printf(format, args...);
#endif
    }

} // namespace mdebug

#endif