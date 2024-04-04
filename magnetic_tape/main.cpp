#include "tape_sort.hpp"
#include "prog_config_parse.hpp"

//-----------------------------------------------------------------------------------------

int main(int argc, char** argv) {

    auto start_time = time_control::chrono_cur_time ();
    try {
        set_up_t prog_config {argc, argv};
        tape_config_t tape_config {prog_config.get_config_file()};

        // tape_config.dump();
        tape_handler_t<int> in_tape(tape_config, prog_config.get_input_tape_file());
        tape_handler_t<int> out_tape(tape_config,
                                     prog_config.get_output_tape_file());

        tape_sort_t<int> sorting {};
        sorting.sort_tape(in_tape, out_tape);

    } catch(std::runtime_error& err) {
        std::cerr << "ERROR: " << err.what() << '\n';
    } catch(...) {
        std::cerr << "Unexpected error\n";
    }

    auto end_time = time_control::chrono_cur_time ();
    time_control::show_run_time(start_time, end_time, "Wall time: ");

    return 0;
}
