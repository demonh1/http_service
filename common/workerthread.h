#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <thread>

namespace utils {

class WorkerThread
{
public:
    WorkerThread();
    virtual ~WorkerThread();

    virtual void Start();
    virtual void Run() = 0;
    void Join();

private:
    void RunThread();

    class ThreadTask
    {
    public:
        ThreadTask(WorkerThread* thr) : thr_(thr) {}

        void operator()() { thr_->RunThread(); }

    private:
        WorkerThread* thr_;
    };

private:
    ThreadTask* thread_task_;
    std::thread* thread_;
};

} // namespace utils

#endif // WORKERTHREAD_H
