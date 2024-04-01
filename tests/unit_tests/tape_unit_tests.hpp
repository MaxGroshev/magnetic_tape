#pragma once

//-----------------------------------------------------------------------------------------

class tape_test : public ::testing::Test {
    protected:
        const char* test_file = "../tests/unit_tests/unit_test_dat/0.dat";
        tape_handler_t<int> my_tape{10, test_file};

    void SetUp() {
    }
};

//-----------------------------------------------------------------------------------------

TEST_F(tape_test, move_forward_test) {

    my_tape.move_tape_forward();
    ASSERT_TRUE(my_tape.get_cur_pos() == 4);

    my_tape.move_tape_forward();
    ASSERT_TRUE(my_tape.get_cur_pos() == 8);
}

TEST_F(tape_test, move_rewind_test) {

    my_tape.move_tape_forward();
    my_tape.move_tape_forward();
    my_tape.rewind_tape();
    auto correct_res = utils::read_bin_file_into_vector<int>(test_file);
    ASSERT_TRUE(correct_res[0] == my_tape.read_from_cell());
}

TEST_F(tape_test, move_backward_test) {

    my_tape.move_tape_forward();
    my_tape.move_tape_forward();
    my_tape.move_tape_forward();

    my_tape.move_tape_backward();
    auto correct_res = utils::read_bin_file_into_vector<int>(test_file);
    ASSERT_TRUE(correct_res[2] == my_tape.read_from_cell());
}

TEST_F(tape_test, multy_reading_test) {

    my_tape.move_tape_forward();
    my_tape.move_tape_forward();
    my_tape.move_tape_forward();

    my_tape.move_tape_backward();
    auto correct_res = utils::read_bin_file_into_vector<int>(test_file);
    ASSERT_TRUE(correct_res[2] == my_tape.read_from_cell());
}

TEST_F(tape_test, writing_test) {

    my_tape.move_tape_forward();
    my_tape.write_to_cell(42);
    std::cout << my_tape.read_from_cell();
    ASSERT_TRUE(42 == my_tape.read_from_cell());
    my_tape.move_tape_forward();

    auto correct_res = utils::read_bin_file_into_vector<int>(test_file);
    ASSERT_TRUE(correct_res[2] == my_tape.read_from_cell());

    my_tape.move_tape_backward();
    ASSERT_TRUE(correct_res[1] == my_tape.read_from_cell());

}
