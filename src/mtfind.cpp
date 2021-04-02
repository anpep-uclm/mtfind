#include <atomic>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include <Shared/Thread.h>
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

    if (args.needle.empty())
        std::cerr << PROGRAM_NAME ": error: nothing to look for in the haystack!" << std::endl;

    return args;
}

int main(int argc, char *argv[])
{
    const auto &args = parse_args(argc, argv);
    std::vector<Thread *> threads;

    for (size_t i = 0; i < args.thread_count; i++) {
        auto thread = new SearchThread { args.file_path, args.needle, args.thread_count };
        threads.push_back(thread);
    }

    for (const auto thread : threads) {
        thread->start();
    }

    for (const auto thread : threads) {
        thread->join();
    }

    return 0;
}