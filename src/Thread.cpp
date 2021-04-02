#include <functional>

#include <Shared/Thread.h>

std::atomic<size_t> Thread::s_thread_count { 0 };

Thread::Thread(std::function<void()> f)
    : m_fn { std::move(f) }
    , m_backing_thread { nullptr }
{
    m_id = s_thread_count++;
}

Thread::~Thread()
{
    delete m_backing_thread;
}

void Thread::start()
{
    m_backing_thread = new std::thread { m_fn };
}

void Thread::join()
{
    m_backing_thread->join();
    dbg("reached join");
}