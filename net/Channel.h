//
// Created by mark on 16-12-8.
//

#ifndef BLACKPANTHER_NET_CHANNEL_H
#define BLACKPANTHER_NET_CHANNEL_H

#include <blackpanther/base/Timestamp.h>
#include <blackpanther/base/Noncopyable.h>

#include <functional>
#include <memory>

namespace blackpanther{
    namespace net{
        class EventLoop;

        class Channel : Noncopyable{
        public:
            typedef std::function<void()> EventCallback;
            typedef std::function<void(Timestamp)> ReadEventCallback;

            Channel(EventLoop *loop, int fd);
            ~Channel();

            void handleEvent(Timestamp receiveTime);

            void setReadCallback(const ReadEventCallback &cb){ readCallback_ = cb; }
            void setWriteCallback(const EventCallback& cb) {  writeCallback_ = cb; }
            void setCloseCallback(const EventCallback& cb) { closeCallback_ = cb; }
            void setErrorCallback(const EventCallback& cb) { errorCallback_ = cb; }

            // rvalue
            void setReadCallback(ReadEventCallback &&cb) { readCallback_ = std::move(cb); }
            void setWriteCallback(EventCallback &&cb) { writeCallback_ = std::move(cb); }
            void setCloseCallback(EventCallback &&cb) { closeCallback_ = std::move(cb); }
            void setErrorCallback(EventCallback &&cb) { errorCallback_ = std::move(cb); }

            void tie(const std::shared_ptr<void> &obj);

            int fd() const { return fd_; }
            int events() const { return events_; }
            void setRevents(int revt) { revents_ = revt; }
            bool isNoneEvent() const { return events_ == kNoneEvent; }

            void enableReading() { events_ |= kReadEvent; update(); }
            void disableReading() { events_ &= ~kReadEvent; update(); }
            void enableWritting() { events_ |= kWriteEvent; update(); }
            void disableWritting() { events_ &= ~kWriteEvent; update(); }
            void disableAll() { events_ = kNoneEvent; update(); }
            bool isWritting() const { return events_ & kWriteEvent; }
            bool isReading() const { return events_ & kReadEvent; }

            int index() { return index_; }
            void setIndex(int idx) { index_ = idx; }

            std::string reventsToString() const;
            std::string eventsToString() const;

            EventLoop* ownerLoop() { return loop_; }
            void remove();

        private:
            static std::string eventsToString(int fd, int ev);

            void update();
            void handleEventWithGuard(Timestamp receiveTime);

            static const int kNoneEvent;
            static const int kReadEvent;
            static const int kWriteEvent;

            EventLoop *loop_;
            const int fd_;
            int events_;
            int revents_;
            int index_;
            bool logHup_;

            std::weak_ptr<void> tie_;
            bool tied_;
            bool eventHandling_;
            bool addedToLoop_;
            ReadEventCallback readCallback_;
            EventCallback writeCallback_;
            EventCallback closeCallback_;
            EventCallback errorCallback_;
        };
    }
}
#endif //BLACKPANTHER_CHANNEL_H
