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

#include <filesystem>
#include <functional>
#include <iostream>
#include <string>

#include <MTFind/SearchThread.h>

SearchThread::SearchThread(std::string path, std::string needle, size_t nthreads, SearchState &search_state)
    : Thread { [this]() { this->run(); } }
    , m_path { std::move(path) }
    , m_needle { std::move(needle) }
    , m_nthreads { nthreads }
    , m_stream { path, std::ios::ate } // initial position will be the end of file
    , m_search_state { search_state }
{
    m_search_state.mutex.lock();
    m_search_state.results[id()] = {};
    m_search_state.mutex.unlock();
}

static std::string previous_word(const std::string &str, off_t start)
{
    if (start > 0) {
        off_t prev_word_start = start - 1,
              prev_word_end = start;

        // skip space characters
        while (prev_word_start > 0 && isspace((uint8_t)str[prev_word_start--]))
            ;

        // obtain printable ASCII characters
        while (prev_word_start > 0) {
            const auto c = static_cast<uint8_t>(str[prev_word_start]);
            if (!isalnum(c) || isspace(c))
                break;
            prev_word_start--;
        }

        return str.substr(prev_word_start, prev_word_end - prev_word_start - 1);
    }

    return {};
}

static std::string next_word(const std::string &str, off_t start, size_t needle_len)
{
    if (start < str.size()) {
        // skip space characters
        off_t next_word_start = start + static_cast<off_t>(needle_len);
        while (next_word_start < str.size() && !isalnum((uint8_t)str[next_word_start++]))
            ;

        // obtain printable ASCII characters
        next_word_start--;
        off_t next_word_end = next_word_start;
        while (next_word_end < str.size()) {
            const auto c = static_cast<uint8_t>(str[next_word_end]);
            if (!isalnum(c) || isspace(c))
                break;
            next_word_end++;
        }

        return str.substr(next_word_start, next_word_end - next_word_start);
    }

    return {};
}

void SearchThread::run()
{
    size_t file_size;
    try {
        file_size = std::filesystem::file_size(m_path);
    } catch (const std::filesystem::filesystem_error &error) {
        dbg(tag(), error.what());
        std::cerr << "could not determine file size: " << error.what() << std::endl;
        return;
    }

    const size_t chunk_length = file_size / m_nthreads;
    off_t chunk_start = static_cast<off_t>(chunk_length) * static_cast<off_t>(this->id()),
          chunk_end = chunk_start + static_cast<off_t>(chunk_length) - 1;

    dbg(tag(), "chunk: [", chunk_start, ", ", chunk_end, "]");

    if (!m_stream.is_open()) {
        dbg(tag(), "stream not open");
        m_stream.open(m_path);
        m_stream.seekg(chunk_start, std::ifstream::beg);
    }

    size_t line_idx = 0;
    for (std::string line; std::getline(m_stream, line) && m_stream.tellg() < chunk_end; line_idx++) {
        off_t needle_idx = -1; // index where the needle was found
        while ((needle_idx = static_cast<off_t>(line.find(m_needle, static_cast<size_t>(needle_idx) + 1))) != std::string::npos) {
            const auto last_character = static_cast<uint8_t>(line[needle_idx + m_needle.size()]);
            if (isalnum(last_character)) {
                // ignore this occurrence if whatever character immediately after the needle is alphanumeric
                // No bounds check is needed because after C++03, line[line.size()] is guaranteed to be '\0'.
                continue;
            }

            const auto prev { previous_word(line, needle_idx) },
                next { next_word(line, needle_idx, m_needle.size()) };
            dbg(tag(), "found at ", line_idx, ":", needle_idx, " -- ", prev, " ", m_needle, " ", next);

            auto snippet = prev;
            snippet.append(" ");
            snippet.append(m_needle);
            snippet.append(" ");
            snippet.append(next);

            m_search_state.mutex.lock();
            m_search_state.results[id()].push_back({ id(), line_idx, needle_idx, needle_idx + static_cast<off_t>(m_needle.size()), snippet });
            m_search_state.mutex.unlock();
        }
    }

    dbg(tag(), "last line index: ", line_idx);
    m_search_state.mutex.lock();
    m_search_state.line_offsets[id()] = line_idx;
    m_search_state.mutex.unlock();
}