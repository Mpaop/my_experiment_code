#include <iostream>
#include <thread>
#include <string>
#include <future>
#include <chrono>
#include <memory>
#include "thread_guard.h"
#include "threaded_stream.h"

std::promise<std::string> pMessage_;
bool doProcess_;

class input_task
{
    std::string message_;

public:
    void operator()()
    {
        while (doProcess_)
        {
            {
                tos << "InputTask: Waiting for input";
                tis >> message_;
                pMessage_.set_value(message_);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    input_task() : message_() {}
};

class output_task
{
    std::shared_future<std::string> pMessageFtr_;
public:
    void operator()()
    {
        pMessageFtr_ = std::move(pMessage_.get_future());
        while (doProcess_)
        {
            std::string mes = pMessageFtr_.get();
            tos << "OutputTask: Received message: " << mes;
            if (mes == "quit") doProcess_ = false;

            pMessage_ = std::promise<std::string>();
            pMessageFtr_ = std::move(pMessage_.get_future());
        }
    }
};

int main()
{
    doProcess_ = true;
    pMessage_ = std::promise<std::string>();

    input_task iTask;
    std::thread iTh(iTask);
    thread_guard iGu(iTh);

    output_task oTask;
    std::thread oTh(oTask);
    thread_guard oGu(oTh);

    return 0;
}