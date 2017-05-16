#include "threadpool.h"
#include "scope_guard.h"


namespace utils {

ThreadPool::ThreadPool(uint32_t queue_size) : stoped_(1), queue_(queue_size)
{

}

bool ThreadPool::SetThreads(uint32_t threads_count)
{
    if(threads_count == 0) return false;

    const size_t size = workers_.size();
    if(size < threads_count)
    {
        // Add workers
        for (uint32_t i = 0; i < threads_count - size; ++i)
        {
            WorkerPtr worker = std::make_shared<Worker>(this, queue_);
            workers_.emplace((unsigned long)worker.get(), worker);
            worker->Start();

        }
        return true;
    }

    if(workers_.size() > threads_count)
    {
        queue_.Push(new StopThreadPoolMessage(size - threads_count));
    }

    return true;

}

void ThreadPool::Start(uint32_t threads_count)
{
    stoped_.store(0,std::memory_order_relaxed);

    for (uint32_t i = 0; i < threads_count; ++i)
    {
        WorkerPtr worker = std::make_shared<Worker>(this, queue_);
        workers_.emplace((unsigned long)worker.get(), worker);
        worker->Start();
    }

}

void ThreadPool::Stop()
{
    stoped_.store(1,std::memory_order_relaxed);
    size_t size = 0;

    {
        lock_t lock(locker_);
        size = workers_.size();

        if(size == 0) return;
    }

    queue_.Push(new StopThreadPoolMessage(size));

    {
        lock_t lock(locker_);
        event_.wait(lock);
    }

    {
        lock_t lock(locker_);
        for( auto&& ptr : stoplist_)
        {
            ptr->Join();
        }
    }
}

void ThreadPool::AddTask(ITask* task, Priority priority)
{
    if( stoped_.load(std::memory_order_relaxed) == 0 )
    {
        queue_.Push(new TaskMessage(task, priority));
    }

}

void ThreadPool::RemoveWorker(unsigned long id)
{
    {
        lock_t lock(locker_);
        auto it = workers_.find(id);
        if(it != workers_.end())
        {
            if( 0 != stoped_.load(std::memory_order_relaxed) )
            {
                stoplist_.push_back(it->second);
            }
            workers_.erase(it);
        }

        if( !workers_.empty() ) return;
    }

    if( 0 != stoped_.load(std::memory_order_relaxed) )
    {
        event_.notify_all();
    }

}

/////////////////////////////////////////////

void ThreadPool::Worker::Run()
{
    auto sg = make_scope_guard(std::bind(&ThreadPool::RemoveWorker, pool_, (unsigned long)this));
    queue_.AddThreadId(std::this_thread::get_id());

    for(;;)
    {
        if ( Message* msg = const_cast< Message* >(queue_.Pop()) )
        {
            int msg_id = msg->id();

            if( msg_id == MSGID_THREAD_POOL_TASK )
            {
                TaskMessage* taskMsg = static_cast< TaskMessage* >(msg);
                ITask* task = taskMsg->GetTask();
                task->RunTask();
                delete task;
                msg->Release();
                continue;
            }
            if( msg_id == MSGID_STOP_THREAD_POOL )
            {
                size_t count = ( static_cast<StopThreadPoolMessage*>(msg) )->DecCount();
                if(count != 0)
                {
                    pool_->queue_.Push( new StopThreadPoolMessage(count) );
                }
                msg->Release();
                break;
            }

            msg->Release();
        }
    }
}

} // namespace utils
