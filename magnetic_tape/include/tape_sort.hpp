#pragma once

#include "tape.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <queue>

//-----------------------------------------------------------------------------------------

template<typename T>
class tape_sort_t {
    private:
        std::string tmp_tapes_dir_ = "../tmp_tapes/";
        std::string output_tape_;
        int n_of_tapes_ = 0;
        std::queue<tape_handler_t<T>> tmp_tapes_;
    public:
        tape_sort_t(const char* output_tape   = "../res",
                    const char* tmp_tapes_dir = "../tmp_tapes/") :
            tmp_tapes_dir_(tmp_tapes_dir),
            output_tape_(output_tape) {};

        void sort_tape(tape_handler_t<T>& tape) {
            tape.rewind_tape();
            sort_by_parts(tape);

            while (tmp_tapes_.size() > 1) {
                for (int i = 0; i < tmp_tapes_.size(); i += 2) {
                    auto fst_tape = std::move(tmp_tapes_.front());
                    tmp_tapes_.pop();
                    tape_handler_t<T> snd_tape {};
                    std::cout << "Size: " << tmp_tapes_.size() << std::endl;
                    if (i <= tmp_tapes_.size() - 1) {
                        snd_tape = std::move(tmp_tapes_.front());
                        tmp_tapes_.pop();
                    }
                    merge_tapes(fst_tape, snd_tape);
                }
            }
            tmp_tapes_.front().rewind_tape();
            std::vector<T> vec = tmp_tapes_.front().read_data_from_tape();
            utils::dump_vect(vec);
            // return tmp_tapes_.front();
        }
        void sort_by_parts(tape_handler_t<T>& tape) {
            while (!tape.reached_end_of_tape()) {
                auto data = tape.read_data_from_tape();
                std::sort(data.begin(), data.end());
                tape_handler_t<T> tmp_tape {tape.get_ram_size(), data.size() * sizeof(T),
                                    (tmp_tapes_dir_ + std::to_string(n_of_tapes_ + 1)).c_str()};
                tmp_tape.write_data_on_tape(data);
                tmp_tape.rewind_tape();
                tmp_tapes_.push(std::move(tmp_tape));
                n_of_tapes_++; //consistent condition
            }
        }

        void merge_tapes(tape_handler_t<T>& fst_tape, tape_handler_t<T>& snd_tape) {
            if (snd_tape.get_size() == 0)
                return;

            size_t total_size = fst_tape.get_size() + snd_tape.get_size();
            tape_handler_t<T> merge_tape {fst_tape.get_ram_size(), total_size,
                            (tmp_tapes_dir_ + std::to_string(n_of_tapes_ + 1)).c_str()};
            while (!fst_tape.reached_end_of_tape() && !snd_tape.reached_end_of_tape()) {
                T fst_elem = fst_tape.read_from_cell();
                T snd_elem = snd_tape.read_from_cell();
                merge_tape.write_and_move_forward(std::min(fst_elem, snd_elem));
                if (fst_elem < snd_elem) {
                    fst_tape.move_tape_forward();
                }
                else
                    snd_tape.move_tape_forward();
            }

            if (fst_tape.reached_end_of_tape())
                merge_tape.copy_from_tape(snd_tape);
            else if (snd_tape.reached_end_of_tape())
                merge_tape.copy_from_tape(fst_tape);

            std::cout << "I am Here:  " << fst_tape.cur_pos_ << " " << snd_tape.cur_pos_ << std::endl;

            n_of_tapes_++;
            merge_tape.rewind_tape();
            tmp_tapes_.push(std::move(merge_tape));
        }
};

//-----------------------------------------------------------------------------------------
