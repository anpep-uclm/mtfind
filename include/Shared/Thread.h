#pragma once

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

class Thread {
protected:
    std::ostream &dbg_one(std::ostream &os)
    {
        return os;
    }

    template <class Arg0, class... Args>
    std::ostream &dbg_one(std::ostream &os, const Arg0 &arg0, const Args &...args)
    {
        os << arg0;
        return dbg_one(os, args...);
    }

    template <class... Args>
    std::ostream &dbg(std::ostream &os, const Args &...args)
    {
        return dbg_one(os, args...);
    }

    template <class... Args>
    std::ostream &dbg(const Args &...args)
    {
        static std::mutex mutex;
        const std::lock_guard<std::mutex> guard { mutex };
        return dbg(std::cout, "[thread ", id(), "]\t", args..., '\n');
    }

public:
    explicit Thread(std::function<void()> f);
    Thread(const Thread &) = delete;
    ~Thread();

    static size_t thread_count() { return s_thread_count; }
    size_t id() const { return m_id; }

    void start();
    void join();

private:
    static std::atomic<size_t> s_thread_count;
    size_t m_id;

    const std::function<void()> m_fn;
    std::thread *m_backing_thread;
};
