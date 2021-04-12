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

#if DEBUG
static std::ostream &dbg_one(std::ostream &os)
{
    return os;
}

template <class Arg0, class... Args>
static std::ostream &dbg_one(std::ostream &os, const Arg0 &arg0, const Args &...args)
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
    return dbg(std::cout, args..., '\n');
}
#else
template <class... Args>
constexpr std::ostream &dbg(const Args &...args)
{
    return std::cout;
}
#endif