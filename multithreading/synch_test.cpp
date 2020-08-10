#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>
#include <condition_variable>
#include "thread_guard.h"
#include "threaded_stream.h"

std::string message_;
std::mutex m_;
std::condition_variable condVar_;
bool doProcess_;
bool receiveInput_;

class input_task
{
public:
    void operator() ()
    {
        while(doProcess_)
        {
            {
                tos << "InputTask: Waiting for input";
                std::lock_guard lock(m_);
                tis >> message_;
                receiveInput_ = true;
            }

            condVar_.notify_one();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
};

class output_task
{
public:
    void operator() ()
    {
        while (doProcess_)
        {
            tos << "OutputTask: Sleeping until input";
            std::unique_lock uLock(m_);
            condVar_.wait(uLock, [] { return receiveInput_; });
            
            tos << "OutputTask: Received message: " << message_;
            receiveInput_ = false;
            if (message_ == "quit") doProcess_ = false;
            uLock.unlock();
        }
    }
};

int main()
{
    doProcess_ = true;
    receiveInput_ = false;

    input_task iTask;
    std::thread iTh(iTask);
    thread_guard iGu(iTh);

    output_task oTask;
    std::thread oTh(oTask);
    thread_guard oGu(oTh);


    return 0;
}