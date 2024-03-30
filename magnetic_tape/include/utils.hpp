#pragma once

#include <vector>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include "time_control.hpp"


namespace fs = std::filesystem;

//-----------------------------------------------------------------------------------------

namespace utils {

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

        if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) {
            files.push_back(std::string(path));
        }
    }

    return files;
}

std::string read_from_file_into_string(const char* path) {
    std::string data;
    std::ifstream data_file;

    data_file.open(path);
    if (!data_file.good()) {
        throw std::runtime_error("Input file does not exist\n");
    }
    data_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream data_stream;
    data_stream << data_file.rdbuf();
    data_file.close();
    data = data_stream.str();
    return data;
}

ssize_t get_file_size (const char* path) {
    struct stat file;
    stat(path, &file);

    return file.st_size;
}

}

