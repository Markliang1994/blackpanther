//
// Created by mark on 16-12-21.
//

#include <blackpanther/net/EventLoopThread.h>

#include <blackpanther/net/EventLoop.h>

using namespace blackpanther;
using namespace blackpanther::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
                                 const std::string& name)
        : loop_(nullptr),
          exiting_(false),
          thread_(std::bind(&EventLoopThread::threadFunc, this), name),
          mutex_(),
          cond_(mutex_),
          callback_(cb)
{
}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if(loop_ != nullptr){
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    assert(!thread_.started());
    thread_.start();

    {
        MutexLockGuard lock(mutex_);
        while(loop_ == nullptr)
            cond_.wait();
    }
    return loop_;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    if(callback_){
        std::cout << "SHit" << std::endl;
        callback_(&loop);
    }
    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.notify();
    }
    loop.loop();
    loop_ = nullptr;
}