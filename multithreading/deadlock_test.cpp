#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include "thread_guard.h"
#include "threaded_ostream.h"

class deadlock_obj
{
public:
    int32_t data_;
    std::mutex m_;

    deadlock_obj(int32_t d) : data_(d) {}
};

deadlock_obj dlo1 = { 10 }, dlo2 = { 20 };

class deadlock_task1
{
public:
    void operator () ()
    {
        tos << "Locking dlo1 mutex";
        std::lock_guard g1(dlo1.m_);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        tos << "Locking dlo2 mutex";
        std::lock_guard g2(dlo2.m_);
        tos << "Locked both objects";
    }
};

class deadlock_task2
{
public:
    void operator () ()
    {
        tos << "Locking dlo2 mutex";
        std::lock_guard g2(dlo2.m_);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        tos << "Locking dlo1 mutex";
        std::lock_guard g1(dlo1.m_);
        tos << "Locked both objects";
    }
};

int main()
{
    std::cout << "main thread id = " << std::this_thread::get_id() << "\n";

    deadlock_task1 task1;
    std::thread t1(task1);
    thread_guard tg1(t1);

    deadlock_task2 task2;
    std::thread t2(task2);
    thread_guard tg2(t2);

    return 0;
}