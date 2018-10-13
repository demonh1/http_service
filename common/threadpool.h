#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "workerthread.h"
#include "priority_queue.h"
#include <atomic>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

namespace utils {

struct ITask
{
    virtual void RunTask() = 0;
    virtual ~ITask() = default;
};

struct Task : ITask
{
    explicit Task(const std::function<void()>& task) : task_(task) {}
    void RunTask() override { task_(); }

private:
    std::function<void()> task_;

};

class ThreadPool
{
public:
    explicit ThreadPool(uint32_t queue_size=4);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool() = default;

    void Start(uint32_t threads_count);
    void Stop();

    bool SetThreads(uint32_t threads_count);
    void AddTask(ITask* task, Priority priority = Priority::NORMAL);
private:
    void RemoveWorker(unsigned long id);

    struct Worker : WorkerThread
    {
        Worker(ThreadPool* pool, PriorityQueue& queue) : pool_(pool), queue_(queue){}
        void Run() override;
    private:
        ThreadPool* pool_;
        PriorityQueue& queue_;
    };

    static constexpr int32_t MSGID_THREAD_POOL_TASK = 3323;
    struct TaskMessage : Message {
        TaskMessage(ITask* task, Priority priority) : Message(MSGID_THREAD_POOL_TASK, priority), task_(task) {}
        ITask* GetTask() { return task_; }
    private:
        ITask* task_;
    };

    static constexpr int32_t MSGID_STOP_THREAD_POOL = 3724;
    struct StopThreadPoolMessage : Message {
        StopThreadPoolMessage(size_t count) : Message(MSGID_STOP_THREAD_POOL), count_(count) {}
        size_t DecCount() const { return --count_; }
    private:
        mutable size_t count_;
    };


private:
    using WorkerPtr = std::shared_ptr<Worker>;
    using WorkersMap = std::unordered_map<unsigned long, WorkerPtr>;

    std::atomic<uint32_t> stoped_;
    PriorityQueue queue_;
    WorkersMap workers_;
    std::vector<WorkerPtr> stoplist_;
    mutex_t locker_;
    condvar_t event_;

};

} // namespace utils

#endif // THREADPOOL_H
