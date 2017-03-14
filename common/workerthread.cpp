#include "workerthread.h"

namespace utils {

WorkerThread::WorkerThread() : thread_task_(nullptr), thread_(nullptr)
{

}

WorkerThread::~WorkerThread()
{
    if(thread_)
    {
        delete thread_task_;
        delete thread_;
    }
}


void WorkerThread::Join()
{
    if(thread_ && thread_->joinable())
    {
        thread_->join();
    }
}


void WorkerThread::Start()
{
    if(!thread_)
    {
        thread_task_ = new ThreadTask(this);
        thread_ = new std::thread(*thread_task_);
    }
}

void WorkerThread::RunThread()
{
    Run();
}

} // namespace utils
