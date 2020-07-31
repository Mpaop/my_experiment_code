#include <iostream>
#include <thread>
#include <memory>

std::unique_ptr<int> hello(std::unique_ptr<int> num)
{
    std::cout << "Hello, World!\n";
    std::cout << "num = " << (* num) << "\n";
    * num = 20;
    return std::move(num);
}

class bg_task
{
    std::unique_ptr<int> num_;

    public:
    bg_task(const int & n) : num_(std::make_unique<int>(n)) {}
    bg_task(const bg_task & task) : bg_task(* task.num_) {}
    bg_task(bg_task && task) : num_(std::move(task.num_)) {}

    void operator() ()
    {
        std::cout << "bg_task thread id = " << std::this_thread::get_id() << "\n";
        num_ = hello(std::move(num_));
        std::cout << "num = " << (* num_) << "\n";
    }
};

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

int main()
{
    std::cout << "main thread id = " << std::this_thread::get_id() << "\n";

    bg_task task(10);
    std::thread t(task);
    thread_guard safeTask(t);

    return 0;
}