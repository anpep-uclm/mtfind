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

#include <cmath>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>

#include <MTFind/SearchState.h>
#include <MTFind/SearchThread.h>

#define PROGRAM_NAME "mtfind"

struct Arguments {
    std::string file_path;
    std::string needle;
    size_t thread_count { 1 };
};

static Arguments parse_args(int argc, const char *const argv[])
{
    if (argc != 3 && argc != 4) {
        std::cout << "usage: " PROGRAM_NAME " file needle [nthreads]" << std::endl;
        std::cout << "Positional arguments:" << std::endl;
        std::cout << "  file\tPath to the file to be searched" << std::endl;
        std::cout << "  needle\tTerm or regular expression to be looked for" << std::endl;
        std::cout << "  nthreads\tNumber of search threads (default is 1)" << std::endl;
        std::exit(0);
    }

    Arguments args;

    if (argc == 4) {
        try {
            args.thread_count = static_cast<size_t>(std::stoul({ argv[3] }));
        } catch (const std::exception &ex) {
            std::cerr << PROGRAM_NAME ": error: invalid value for argument `nthreads'" << std::endl;
            std::exit(1);
        }
    }

    args.file_path = argv[1];
    args.needle = argv[2];
    dbg("args\n\tthread_count=", args.thread_count, "\n\tfile_path=", args.file_path, "\n\tneedle=", args.needle);

    if (args.needle.empty())
        std::cerr << PROGRAM_NAME ": error: nothing to look for in the haystack!" << std::endl;

    return args;
}

int main(int argc, char *argv[])
{
    const auto &args = parse_args(argc, argv);
    std::vector<std::unique_ptr<Thread>> threads;

    SearchState search_state;
    for (size_t i = 0; i < args.thread_count; i++) {
        threads.push_back(std::make_unique<SearchThread>(args.file_path, args.needle, args.thread_count, search_state));
    }

    for (const auto &thread : threads) {
        thread->start();
    }

    for (const auto &thread : threads) {
        thread->join();
    }

    for (size_t i = 0, line_off = 0;
         i < args.thread_count;
         line_off += search_state.line_offsets[i], i++) {
        for (const auto &result : search_state.results[i]) {
            printf("[Hilo %u inicio: %ld - final: %ld] :: línea %zu :: ... %s ...\n",
                result.thread_id,
                1 + result.start_idx,
                1 + result.end_idx,
                2 + result.line + line_off,
                result.snippet.c_str());
        }
    }

    return 0;
}