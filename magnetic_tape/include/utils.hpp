#pragma once

#include <vector>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include "time_control.hpp"


//-----------------------------------------------------------------------------------------

namespace utils {

namespace fs = std::filesystem;

template<typename T>
void dump_vect(const std::vector<T>& vect) {
    for (const auto& elem : vect)
        std::cout << elem << " ";
    std::cout << '\n';
}

std::vector<std::string> get_file_name_from_dir(const char* dir_name) {
    std::string path = std::string(fs::current_path()) + "/" + dir_name;
    std::vector<std::string> files {};

    struct stat sb;
    for (const auto& elem_in_dir : fs::directory_iterator(path)) {

        fs::path outfilename = elem_in_dir.path();
        std::string outfilename_str = outfilename.string();
        const char* path = outfilename_str.c_str();

        if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) { //c-style(remove later)
            files.push_back(std::string(path));
        }
    }

    return files;
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
    std::vector<T> data (reinterpret_cast<T*>(char_buf),
                         reinterpret_cast<T*>(char_buf) + f_size / sizeof(T));
    delete [] char_buf;
    data_file.close();
    return data;
}

}

