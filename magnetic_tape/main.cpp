#include "tape.hpp"

//-----------------------------------------------------------------------------------------

int main(int argc, char** argv) {

    auto start_time = time_control::chrono_cur_time ();
    try {
        // set_up_t set_up {argc, argv};
        tape_handler_t<int> my_tape(10, "../tests/end_to_end_tests/my_test_dat/0.dat");
        std::vector<int> data{1, 2, 3, 4};

        my_tape.write_data_on_tape(data);
        // utils::dump_vect(data);
    } catch(std::runtime_error& err) {
        std::cout << "ERROR: " << err.what() << '\n';
    } catch(...) {
        std::cout << "Unexpected error\n";
    }
    auto end_time = time_control::chrono_cur_time ();
    time_control::show_run_time(start_time, end_time, "Wall time: ");

    return 0;
}
