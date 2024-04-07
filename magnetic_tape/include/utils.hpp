#pragma once

#include <vector>
#include <iostream>
#include <filesystem>
#include "time_control.hpp"

//-----------------------------------------------------------------------------------------

namespace utils {

namespace fs = std::filesystem;

template<typename It>
void dump_cont(const It start, const It fin) {
    for (auto iter = start; iter != fin; iter++)
        std::cout << *iter << ' ' << std::endl;
    std::cout << '\n';
}

template<typename T>
std::vector<T> read_bin_file_into_vector(const char* path) {
    std::ifstream data_file(path, std::ios::binary);
    if (!data_file.good()) {
        std::string msg = "Input file for decompression does not exist\n";
        throw std::runtime_error(msg + path);
    }

    size_t f_size = fs::file_size(path);
    char* char_buf = new char[f_size];
    data_file.read(char_buf, f_size);
    std::vector<T> data (std::bit_cast<T*>(char_buf),
                         std::bit_cast<T*>(char_buf) + f_size / sizeof(T));
    delete [] char_buf;
    data_file.close();
    return data;
}

}

