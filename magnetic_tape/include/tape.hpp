#pragma once

#include "utils.hpp"
#include <string>
#include <iostream>

//-----------------------------------------------------------------------------------------

template<typename T>
class tape_handler_t {
    private:
        // set_up_t config;
        std::fstream tape_;
        size_t ram_size_;
        size_t tape_size_ = 0;
        std::streampos cur_pos_  = 0;

        std::streampos set_cur_pos(std::streampos pos) {
            cur_pos_ = pos;
            tape_.seekg(pos);
            return cur_pos_;
        }
    public:
        tape_handler_t(int ram_size, const char* tape_name) :
            ram_size_(ram_size), tape_(tape_name, std::ios::in |
                                       std::ios::out | std::ios::binary) {
                tape_size_ = fs::file_size(tape_name);
                std::cout << tape_size_ << std::endl;
            };

        template<typename Cont>
        void read_data_from_tape(Cont& data);
        template<typename Cont>
        void write_data_on_tape(Cont& data);

        void move_tape_forward() {
            //sleep;

            if (cur_pos_ + std::streamoff(sizeof(int)) > tape_size_)
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

            int prev_pos = cur_pos_;
            tape_.write(reinterpret_cast<char*>(&elem), sizeof(elem));
            set_cur_pos(prev_pos);
            return elem;
        }
        T read_from_cell() {
            //sleep;

            int prev_pos = cur_pos_;
            T elem {};
            tape_.read(reinterpret_cast<char*>(&elem), sizeof(elem));
            set_cur_pos(prev_pos);
            return elem;
        }
        std::streampos get_cur_pos() const {return cur_pos_;};
};

//-----------------------------------------------------------------------------------------

template<typename T>
template<typename Cont>
void tape_handler_t<T>::read_data_from_tape(Cont& data) {
    for(int i = 0; i < std::min(ram_size_, tape_size_); i++) {
        data.push_back(read_from_cell());
        move_tape_forward();
    }
}

template<typename T>
template<typename Cont>
void tape_handler_t<T>::write_data_on_tape(Cont& data) {
    for(int i = 0; i < std::min(data.size(), tape_size_); i++) {
        write_to_cell(data[i]);
        move_tape_forward();
    }
}
