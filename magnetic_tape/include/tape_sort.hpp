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
        int n_of_tapes_ = 0;
        std::queue<tape_handler_t<T>> tmp_tapes_;

//-----------------------------------------------------------------------------------------

        void sort_by_parts(tape_handler_t<T>& tape) {
            while (!tape.reached_end_of_tape()) {
                auto data = tape.read_data_from_tape();
                std::sort(data.begin(), data.end());
                auto config = tape.get_config();
                tape_handler_t<T> tmp_tape {config, data.size() * sizeof(T),
                            (tmp_tapes_dir_ + std::to_string(n_of_tapes_ + 1))};
                tmp_tape.write_data_on_tape(data.begin(), data.end());
                tmp_tape.rewind_tape();
                tmp_tapes_.push(std::move(tmp_tape));
                n_of_tapes_++; //consistent condition
            }
        }

        void merge_tapes(tape_handler_t<T>& fst_tape, tape_handler_t<T>& snd_tape,
                         tape_handler_t<T>& merge_tape) {
            if (snd_tape.get_size() == 0) {
                std::cout << "Returned\n";
                return;
            }

            while (!fst_tape.reached_end_of_tape() &&
                   !snd_tape.reached_end_of_tape()) {

                T fst_elem = fst_tape.read_from_cell();
                T snd_elem = snd_tape.read_from_cell();
                merge_tape.write_and_move_forward(std::min(fst_elem, snd_elem));
                if (fst_elem < snd_elem) {
                    fst_tape.move_tape_forward();
                }
                else
                    snd_tape.move_tape_forward();
            }
            if (fst_tape.reached_end_of_tape()) {
                merge_tape.copy_from_tape(snd_tape);
            }
            else if (snd_tape.reached_end_of_tape())
                merge_tape.copy_from_tape(fst_tape);
        }

        tape_handler_t<T> get_tape_for_merge(tape_handler_t<T>& fst_tape,
                                             tape_handler_t<T>& snd_tape) {

            size_t total_size = fst_tape.get_size() + snd_tape.get_size();
            std::string tape_name = tmp_tapes_dir_ + std::to_string(n_of_tapes_ + 1);
            n_of_tapes_++;

            auto config = fst_tape.get_config();
            return tape_handler_t<T> {config, total_size,
                                      (tape_name).c_str()};
        }

//-----------------------------------------------------------------------------------------

    public:
        tape_sort_t(fs::path tmp_tapes_dir = "../tmp_tapes/") :
            tmp_tapes_dir_(tmp_tapes_dir) {};

        void sort_tape(tape_handler_t<T>& tape, tape_handler_t<T>& res_tape) {
            tape.rewind_tape();
            res_tape.rewind_tape();
            if (tape.get_size() <= tape.get_ram_size()) {
                sort_without_tmp_tape(tape, res_tape);
                return;
            }
            sort_by_parts(tape);

            while (tmp_tapes_.size() > 1) {
                for (int i = 0; i < tmp_tapes_.size(); i += 2) {
                    auto fst_tape = std::move(tmp_tapes_.front());
                    tmp_tapes_.pop();
                    tape_handler_t<T> snd_tape {};
                    if (i <= tmp_tapes_.size()) {
                        snd_tape = std::move(tmp_tapes_.front());
                        tmp_tapes_.pop();
                    }
                    auto tape_for_merge = get_tape_for_merge(fst_tape, snd_tape);
                    merge_tapes(fst_tape, snd_tape, tape_for_merge);

                    tape_for_merge.rewind_tape();
                    tmp_tapes_.push(std::move(tape_for_merge));
                }
                if (tmp_tapes_.size() == 2) {//last iteration of front
                    auto fst_tape = std::move(tmp_tapes_.front());
                    tmp_tapes_.pop();
                    auto snd_tape = std::move(tmp_tapes_.front());
                    tmp_tapes_.pop();
                    merge_tapes(fst_tape, snd_tape, res_tape);
                }
            }
        }
        void sort_without_tmp_tape(tape_handler_t<T>& tape,
                                   tape_handler_t<T>& res_tape) {
            auto data = tape.read_data_from_tape();
            std::sort(data.begin(), data.end());
            res_tape.write_data_on_tape(data.begin(), data.end());
            res_tape.rewind_tape();
        }
};

//-----------------------------------------------------------------------------------------
