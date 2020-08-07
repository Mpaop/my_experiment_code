#pragma once

#include <thread>

class thread_guard
{
    std::thread & t_;
public:
	explicit thread_guard(std::thread & t) : t_(t) {}
	virtual ~thread_guard()
    {
        if(t_.joinable()) t_.join();
    }

    thread_guard(thread_guard const &) = delete;
    thread_guard & operator = (thread_guard const &) = delete;
};