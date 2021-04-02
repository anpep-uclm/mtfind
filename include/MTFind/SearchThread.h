#pragma once

#include <string>
#include <fstream>

#include <Shared/Thread.h>

class SearchThread final : public Thread {
public:
    SearchThread(std::string path, std::string needle, size_t nthreads);
    void run();

private:
    const std::string m_path;
    const std::string m_needle;

    size_t m_nthreads;
    size_t m_start_idx, m_end_idx;

    std::ifstream m_stream;
};