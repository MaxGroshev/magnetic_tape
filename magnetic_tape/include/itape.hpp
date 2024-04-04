#pragma once

#include "utils.hpp"
#include <string>
#include <iostream>
#include <thread>
#include <memory>

#include "tape_config_parse.hpp"

//-----------------------------------------------------------------------------------------

template<typename T>
class itape_t {
    protected:
        tape_config_t config_;
        std::fstream tape_;
        size_t max_count_of_elem_in_ram_;
        size_t tape_size_ = 0;
        std::streampos cur_pos_ = 0;

        virtual std::streampos update_cur_pos(std::streampos pos) = 0;
    public:
        itape_t() :
            tape_size_(0) {};

        itape_t(tape_config_t& config, fs::path tape_name) :
            config_(config),
            max_count_of_elem_in_ram_(config_.get_ram_size() / sizeof(T)),
            tape_(tape_name, std::ios::in | std::ios::out | std::ios::binary) {
                if (max_count_of_elem_in_ram_ < 1) {
                    throw std::runtime_error(
                    "Size of Ram is too little (at least sizeof(T))");
                }
                tape_size_ = fs::file_size(tape_name);
                // std::cout << tape_name << "   " << tape_size_ << std::endl;
        };

        itape_t(tape_config_t& config, size_t size, fs::path tape_name) :
            config_(config),
            max_count_of_elem_in_ram_(config_.get_ram_size() / sizeof(T)),
            tape_size_(size), tape_(tape_name, std::ios::in |
            std::ios::out | std::ios::binary | std::ios::trunc) {
                if (max_count_of_elem_in_ram_ < 1) {
                    throw std::runtime_error(
                    "Size of Ram is too little (at least sizeof(T))");
                }
                // std::cout << "SND: "<<tape_name << "   " << tape_size_ << std::endl;
        };

        itape_t(const char* tape_name) :
            max_count_of_elem_in_ram_(config_.get_ram_size() / sizeof(T)),
            tape_(tape_name, std::ios::in | std::ios::out | std::ios::binary) {
                if (max_count_of_elem_in_ram_ < 1) {
                    throw std::runtime_error(
                    "Size of Ram is too little (at least sizeof(T))");
                }
                tape_size_ = fs::file_size(tape_name);
                // std::cout << tape_name << "   " << tape_size_ << std::endl;
        };


        virtual void move_tape_forward() = 0;
        virtual void move_tape_backward() = 0;
        virtual void rewind_tape() = 0;
        virtual T write_to_cell(T elem) = 0;
        virtual T write_and_move_forward(T elem) = 0;
        virtual std::vector<T> read_data_from_tape() = 0;
        virtual void write_data_on_tape(T* cont, size_t n_elem) = 0;//no way for iterators

        virtual T read_from_cell() = 0;
        virtual ~itape_t() = default;

        virtual void copy_from_tape(itape_t<T>& src) = 0;
        virtual size_t get_ram_size()          const = 0;
        virtual size_t get_size()              const = 0;
        virtual bool reached_end_of_tape()     const = 0;
        virtual std::streampos get_cur_pos()   const = 0;
        virtual tape_config_t get_config()     const = 0;

        virtual std::unique_ptr<itape_t<T>>create(
                tape_config_t& config, fs::path tape_name) = 0;
        virtual std::unique_ptr<itape_t<T>>create(
                tape_config_t& config, size_t size, fs::path tape_name) = 0;
};

//-----------------------------------------------------------------------------------------
