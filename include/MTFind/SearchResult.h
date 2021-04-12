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

#include <Shared/Thread.h>
#include <string>
#include <utility>

struct SearchResult {
    /** ID of the thread that found the match */
    ThreadID thread_id;
    /** Line where the thread found the match */
    size_t line;
    /** Start and end offsets within the line */
    off_t start_idx, end_idx;
    /** Text surrounding the match */
    std::string snippet;

    /**
     * Represents a search result
     * @param thread_id ID of the thread that found the match
     * @param line Line where the thread found the match
     * @param start_idx Start offset within the line
     * @param end_idx End offset within the line
     * @param snippet Text surrounding the match
     */
    SearchResult(ThreadID thread_id, size_t line, off_t start_idx, off_t end_idx, std::string snippet)
        : thread_id(thread_id)
        , line(line)
        , start_idx(start_idx)
        , end_idx(end_idx)
        , snippet(std::move(snippet))
    {
    }
};