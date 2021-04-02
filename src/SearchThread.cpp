#include <filesystem>
#include <iostream>
#include <string>
#include <functional>

#include <MTFind/SearchThread.h>

SearchThread::SearchThread(std::string path, std::string needle, size_t nthreads)
    : Thread { [this]() { this->run(); } }
    , m_path { std::move(path) }
    , m_needle { std::move(needle) }
    , m_nthreads { nthreads }
    , m_stream { path, std::ios::ate } // initial position will be the end of file
{
}

void SearchThread::run()
{
    const size_t file_size = std::filesystem::file_size(m_path),
                 chunk_length = file_size / m_nthreads;
    size_t chunk_start = chunk_length * id(),
           chunk_end = chunk_start + chunk_length;
    m_stream.seekg(chunk_start, std::ifstream::beg);
}
