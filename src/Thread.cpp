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

#include <chrono>
#include <functional>
#include <future>

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
    this->join();
    delete m_backing_thread;
}

void Thread::start()
{
    m_backing_thread = new std::thread { m_fn };
}

void Thread::join()
{
    if (m_backing_thread->joinable()) {
        dbg(tag(), "thread still running -- waiting");
        m_backing_thread->join();
    }
}