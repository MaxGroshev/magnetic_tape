#pragma once

#include "utils.hpp"
#include <string>
#include <iostream>
// #include <threasd>

#include "tape_config_parse.hpp"

//-----------------------------------------------------------------------------------------

template<typename T>
class tape_handler_t {
    public:
        tape_config_t config_;
        std::fstream tape_;
        size_t max_count_of_elem_in_ram_;
        size_t tape_size_ = 0;
        std::streampos cur_pos_ = 0;

        std::streampos set_cur_pos(std::streampos pos) {
            cur_pos_ = pos;
            tape_.seekg(pos);
            return cur_pos_;
        }
    public:

        tape_handler_t() : //think how to improve
            tape_size_(0) {};

        tape_handler_t(tape_config_t& config, fs::path tape_name) :
            config_(config),
            max_count_of_elem_in_ram_(config_.get_ram_size() / sizeof(T)),
            tape_(tape_name, std::ios::in | std::ios::out | std::ios::binary) {
                if (max_count_of_elem_in_ram_ < 1) {
                    throw std::runtime_error("Size of Ram is too little (at least sizeof(T))");
                }
                tape_size_ = (fs::file_size(tape_name) / sizeof(T)) * sizeof(T); //improve
                std::cout << tape_name << "   " << tape_size_ << std::endl;
        };

        tape_handler_t(tape_config_t& config, size_t size, fs::path tape_name) :
            config_(config),
            max_count_of_elem_in_ram_(config_.get_ram_size() / sizeof(T)),
            tape_size_(size), tape_(tape_name, std::ios::in     |
            std::ios::out | std::ios::binary | std::ios::trunc) {
                if (max_count_of_elem_in_ram_ < 1) {
                    throw std::runtime_error("Size of Ram is too little (at least sizeof(T))");
                }
                // rewind_tape();
                std::cout << tape_name << "   " << tape_size_ << std::endl;
        };

        tape_handler_t(const char* tape_name) :
            max_count_of_elem_in_ram_(config_.get_ram_size() / sizeof(T)),
            tape_(tape_name, std::ios::in | std::ios::out | std::ios::binary) {
                if (max_count_of_elem_in_ram_ < 1) {
                    throw std::runtime_error("Size of Ram is too little (at least sizeof(T))");
                }
                tape_size_ = (fs::file_size(tape_name) / sizeof(T)) * sizeof(T); //improve
                std::cout << tape_name << "   " << tape_size_ << std::endl;
        };

        std::vector<T> read_data_from_tape();
        template<typename It>
        void write_data_on_tape(It start, It fin);

        void move_tape_forward() {
            //sleep;

            if (reached_end_of_tape())
                throw std::runtime_error("Attempt to access elem out of tape");
            set_cur_pos(tape_.tellg() + std::streamoff(sizeof(T)));
        }
        void move_tape_backward() {
            //sleep;

            if (cur_pos_ - std::streamoff(sizeof(int)) < 0)
                throw std::runtime_error("Attempt to access elem out of tape");

            set_cur_pos(tape_.tellg() - std::streamoff(sizeof(T)));
        }
        void rewind_tape() {
            //sleep;

            set_cur_pos(0);
        }
        T write_to_cell(T elem) {
            //sleep;

            auto prev_pos = cur_pos_;
            tape_.write(reinterpret_cast<char*>(&elem), sizeof(elem));
            set_cur_pos(prev_pos);
            return elem;
        }
        T write_and_move_forward(T elem) {
            write_to_cell(elem);
            if (!reached_end_of_tape())
                move_tape_forward();
            return elem;
        }
        T read_and_move_forward() {
            T elem = read_from_cell();
            if (!reached_end_of_tape())
                move_tape_forward();
            return elem;
        }
        T read_from_cell() {
            //sleep;

            auto prev_pos = cur_pos_;
            T elem {};
            tape_.read(reinterpret_cast<char*>(&elem), sizeof(elem));
            set_cur_pos(prev_pos);
            return elem;
        }
        void copy_from_tape(tape_handler_t<T>& src);
        std::streampos get_cur_pos() const {return cur_pos_;};
        size_t get_ram_size() const {return config_.get_ram_size();};
        size_t get_size() const {return tape_size_;};
        tape_config_t get_config() const {return config_;};
        bool reached_end_of_tape() const {
            return (cur_pos_ == tape_size_);
        }
};

//-----------------------------------------------------------------------------------------

template<typename T>
std::vector<T> tape_handler_t<T>::read_data_from_tape() {
    std::vector<T> data {};
    for(int i = 0; (i < max_count_of_elem_in_ram_) && !reached_end_of_tape(); i++) {
        data.push_back(read_from_cell());
        move_tape_forward();
    }
    return data;
}

template<typename T>
template<typename It>
void tape_handler_t<T>::write_data_on_tape(It start, It fin) {
    for(auto iter = start; (iter != fin) && !reached_end_of_tape(); iter++) {
        write_to_cell(*iter);
        move_tape_forward();
    }
}

template<typename T>
void tape_handler_t<T>::copy_from_tape(tape_handler_t<T>& src) {
    while (!reached_end_of_tape() && !src.reached_end_of_tape()) {
        write_to_cell(src.read_from_cell());
        move_tape_forward();
        src.move_tape_forward();
    }
}
