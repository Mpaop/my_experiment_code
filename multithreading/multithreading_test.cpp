#include <iostream>
#include <sstream>
#include <utility>
#include <thread>
#include <memory>
#include <mutex>

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

int32_t SharedResource;
std::mutex SRMutex;

void IncSharedResource()
{
    std::lock_guard guard(SRMutex);
    ++ SharedResource;
    tos << "Incremented Shared Resource from thread: " << std::this_thread::get_id() <<  "\nSharedResource = " << SharedResource;
}

class bg_task
{
    std::shared_ptr<std::string> str_;
    uint32_t accessIdx_;

    public:
    bg_task(std::shared_ptr<std::string> str, uint32_t idx) : str_(str), accessIdx_(idx) {}
    bg_task(const bg_task & task) : bg_task(task.str_, task.accessIdx_) {}
    bg_task(bg_task && task) : str_(std::move(task.str_)), accessIdx_(std::move(task.accessIdx_)) {}

    void operator() ()
    {
        tos << "bg_task thread id = " << std::this_thread::get_id();
        (*str_)[accessIdx_] = 'z';
        tos << "thread: " << std::this_thread::get_id() << " modified string. string = " << str_->c_str();
        IncSharedResource();
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

    SharedResource = 0;

    std::shared_ptr<std::string> str = std::make_shared<std::string>("aaaaaaaaa");

    bg_task task(str, 1);
    std::thread t(task);
    thread_guard safeTask(t);

    bg_task task2(str, 3);
    std::thread t2(task2);
    thread_guard safeTask2(t2);

    return 0;
}
