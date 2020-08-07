#pragma once

#include <mutex>
#include <iostream>
#include <sstream>

class threaded_ostringstream : public std::ostringstream
{
private:
    static std::mutex tosMutex_;

public:
    threaded_ostringstream() = default;

    virtual ~threaded_ostringstream()
    {
        std::lock_guard guard(tosMutex_);
        std::cout << this->str() << "\n" << std::flush;
    }
};

std::mutex threaded_ostringstream::tosMutex_ = {};
#define tos threaded_ostringstream {}
