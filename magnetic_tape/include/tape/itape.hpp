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
    public:
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
