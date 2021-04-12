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

#include <mutex>
#include <unordered_map>

#include <MTFind/SearchResult.h>
#include <Shared/Thread.h>

struct SearchState {
    /** Lock for concurrent access to data structures */
    std::mutex mutex;
    /** Search results */
    std::unordered_map<ThreadID, std::vector<SearchResult>> results;
    /** Last line index of each thread */
    std::unordered_map<ThreadID, uint32_t> line_offsets;

    SearchState()
        : mutex {}
        , results {}
        , line_offsets {}
    {
    }
};