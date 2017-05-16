#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "sync.h"
#include "msg.h"

#include <queue>
#include <vector>
#include <set>

namespace utils {

class PriorityQueue
{
public:
    PriorityQueue(uint32_t size = 0) : queue_size_(size), count_(0), queue_(16) {}
    ~PriorityQueue() = default;

    void Push(const Message* msg)
    {
        int prio = msg->priority();
        uint32_t count = 0;

        for(;;)
        {
            {
                lock_t lock(locker_);
                if( 0 == queue_size_ || count_ < queue_size_ || CheckThreadId( std::this_thread::get_id() ) )
                {
                    msg_queue_t& q = queue_[prio];
                    q.push(msg);
                    count = ++count_;
                    break;
                }
            }

            lock_t lock(locker_);
            overload_event_.wait(lock);
        }

        if(count == 1)
        {
            event_.notify_all();
        }
    }

    const Message* Pop()
    {
        Message* msg = nullptr;
        lock_t lock(locker_);
        if(count_ != 0)
        {
            for(auto it = queue_.rbegin(), end = queue_.rend(); it != end; ++it)
            {
                msg_queue_t& q = *it;
                if(!q.empty())
                {
                    msg = const_cast<Message*>(q.front());
                    q.pop();
                    --count_;

                    if(queue_size_ != 0 && count_ == queue_size_ - 1)
                    {
                        overload_event_.notify_all();
                    }
                    break;
                }
            }
        }

        if(!msg)
            event_.wait(lock);

        return msg;
    }

    void AddThreadId(std::thread::id id)
    {
        lock_t lock(locker_);
        thread_ids_.emplace(id);
    }

    void SetQueueSize(uint32_t size)
    {
        lock_t lock(locker_);
        queue_size_ = size;
    }

private:
    bool CheckThreadId(const std::thread::id& id)
    {
        return thread_ids_.find(id) != thread_ids_.end();
    }

    using msg_queue_t = std::queue< const Message* >;
    volatile uint32_t queue_size_, count_;

    std::vector<msg_queue_t> queue_;
    mutex_t locker_;
    condvar_t event_, overload_event_;
    std::set<std::thread::id> thread_ids_;
};

} // namespace utils

#endif // PRIORITY_QUEUE_H
