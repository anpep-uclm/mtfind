// mtfind(1) -- Half-assed multithread grep clone
// Copyright (c) 2021 Ángel Pérez <angel@ttm.sh>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

#include <Shared/Debug.h>

using ThreadID = uint32_t;

class Thread {
protected:
public:
    explicit Thread(std::function<void()> f);
    ~Thread();

    static size_t thread_count() { return s_thread_count; }
    ThreadID id() const { return m_id; }

    void start();
    void join();

protected:
    auto tag() const { return "[thread " + std::to_string(id()) + "] "; }

private:
    static std::atomic<ThreadID> s_thread_count;
    ThreadID m_id;

    const std::function<void()> m_fn;
    std::thread *m_backing_thread;
};
