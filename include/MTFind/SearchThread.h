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

#include <fstream>
#include <string>

#include <MTFind/SearchState.h>
#include <Shared/Thread.h>

class SearchThread final : public Thread {
public:
    /**
     * Thread that will look for the specific string of characters in a text file
     * @param path Path of the source file
     * @param needle String of characters to be looked for
     * @param nthreads Total number of threads for splitting up the file in chunks
     * @param search_state Shared search state
     */
    SearchThread(std::string path, std::string needle, size_t nthreads, SearchState &search_state);

private:
    /**
     * Performs the search
     */
    void run();

    /** Source file path */
    const std::string m_path;

    /** String of characters to be looked for */
    const std::string m_needle;

    /** Number of search threads */
    size_t m_nthreads;

    /** Input stream */
    std::ifstream m_stream;

    /** Shared search state */
    SearchState &m_search_state;
};