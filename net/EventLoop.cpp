//
// Created by mark on 16-12-6.
//

#include <blackpanther/net/EventLoop.h>
#include <blackpanther/net/Channel.h>
#include <blackpanther/net/TimerQueue.h>

#include <blackpanther/base/Logging.h>
#include <blackpanther/net/Poller.h>

#include <langinfo.h>

#include <signal.h>
#include <sys/eventfd.h>

using namespace blackpanther;
using namespace blackpanther::net;


namespace {
    __thread EventLoop *t_loopInThisThread = 0;

    const int kPollTimeMs = 10000;

    int createEventfd() {
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if(evtfd < 0){
            LOG_SYSERR << "Failed in eventfd";
            abort();
        }
        return evtfd;
    }
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}

EventLoop::EventLoop():
        looping_(false),
        quit_(false),
        eventHandling_(false),
        callingPendingFunctors_(false),
        iteration_(0),
        threadId_(CurrentThread::tid()),
        poller_(Poller::newDefaultPoller(this)),
        timerQueue_(new TimerQueue(this)),
        wakeupFd_(createEventfd()),
        wakeupChannel_(new Channel(this, wakeupFd_)),
        currentActiveChannel_(nullptr)

    {

        LOG_DEBUG << "EventLoop created " << this << " in thread " << threadId_;
        if(t_loopInThisThread){
            LOG_FATAL << "Another EventLoop " << t_loopInThisThread << " exists in this thread " << threadId_;
        }
        else{
            t_loopInThisThread = this;
        }
        wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
        wakeupChannel_->enableReading();
}

void EventLoop::loop(){
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    LOG_TRACE << "EventLoop " << this << " start looping.";

    while(!quit_){
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        ++iteration_;
        if(Logger::logLevel() <= Logger::TRACE)
            printActiveChannels();

        eventHandling_ = true;
        for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it){
            currentActiveChannel_ = *it;
            currentActiveChannel_->handleEvent(pollReturnTime_);
        }
        currentActiveChannel_ = nullptr;
        eventHandling_ = false;
        doPendingFunctors();
    }

    LOG_TRACE  << "EventLoop " << this << " stop looping";
    looping_ = false;
}

void EventLoop::quit() {
    quit_ = true;
    if(!isInLoopThread()){
        wakeup();
    }
}

void EventLoop::runInLoop(const Functor &cb) {
    if(isInLoopThread()) {
        cb();
    }
    else
        queueInLoop(cb);
}

void EventLoop::queueInLoop(const Functor &cb) {
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    }
    if(!isInLoopThread() || callingPendingFunctors_)
        wakeup();
}

size_t EventLoop::queueSize() const {
    return pendingFunctors_.size();
}

TimerId EventLoop::runAt(const Timestamp &time, const TimerCallback &cb) {
    return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback &cb) {
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback &&cb) {
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(cb, time, interval);
}

EventLoop::~EventLoop() {
    LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_ << " destruct in thread" << CurrentThread::tid();

    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::abortNotInLoopThread() {
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
                                                                << " was created in thread " << threadId_
                                                                                             << ", But current thread id is: " << CurrentThread::tid();
}

void EventLoop::wakeup(){
    uint64_t  one = 1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof(one));
    if(n != sizeof(one)){
        LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead if 8";
    }
}

void EventLoop::updateChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();

    if(eventHandling_){
        assert(currentActiveChannel_ == channel ||
            std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
    }
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return poller_->hasChannel(channel);
}
void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof(one));
    if(n != sizeof(one)){
        LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
}
void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(size_t i = 0; i < functors.size(); ++i){
        functors[i]();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::printActiveChannels() const {
    for(ChannelList::const_iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it){
        const Channel *ch = *it;
        LOG_TRACE << "{" << ch->reventsToString() << "} ";
    }
}