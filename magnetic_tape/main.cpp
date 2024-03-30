// #include "lzw.hpp"
#include "utils.hpp"

//-----------------------------------------------------------------------------------------

int main(int argc, char** argv) {

    auto start_time = time_control::chrono_cur_time ();
    try {
        // set_up_t set_up {argc, argv};


    } catch(std::runtime_error& err) {
        std::cout << "ERROR: " << err.what() << '\n';
    } catch(...) {
        std::cout << "Unexpected error\n";
    }
    auto end_time = time_control::chrono_cur_time ();
    time_control::show_run_time(start_time, end_time, "Wall time: ");

    return 0;
}
