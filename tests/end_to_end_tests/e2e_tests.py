import subprocess
import argparse
import os
import sys
from   subprocess import Popen, PIPE, STDOUT

# -----------------------------------------------------------------------------------------

class TERMINAL_COLORS:
        PURPLE    = '\033[95m'
        OKBLUE    = '\033[94m'
        OKCYAN    = '\033[96m'
        OKGREEN   = '\033[92m'
        YELLOW    = '\033[33m'
        BLUE      = '\033[36m'
        GREEN     = '\033[32m'

        WARNING   = '\033[93m'
        ERROR     = '\033[91m'
        RED       = '\033[91m'
        DEFAULT   = '\033[0m'
        BOLD      = '\033[1m'
        UNDERLINE = '\033[4m'


data_files_names = []

path2dir = os.path.dirname(os.path.abspath(__file__))
path2dir_len = len(path2dir)

# -----------------------------------------------------------------------------------------

def get_files_names(files_names, dir_path):
    # print (dir_path)
    data_files_counter = 0

    for path in os.listdir(dir_path):
        if os.path.isfile(os.path.join(dir_path, path)):
            data_files_counter += 1
            files_names.append(os.path.join(dir_path, path))

# -----------------------------------------------------------------------------------------

def check_output_data(n_of_test, stdout_data, correct_res, test_case):
    try:
        if stdout_data == correct_res:
            print(TERMINAL_COLORS.GREEN            + \
                f"Test {n_of_test} IS PASSED:\n"   + \
                f"Prog output = {stdout_data}\n"   + \
                f"File:  {test_case}"              + \
            TERMINAL_COLORS.DEFAULT
            )
            print('—————————————————————————END_OF_TEST————————————————————————\n\n')
            return True
        else:
            print(TERMINAL_COLORS.ERROR            + \
                f"Test {n_of_test} IS FAILED:\n"   + \
                f"Prog output   = {stdout_data}\n" + \
                f"Expected      = {correct_res}\n"   + \
                f"File:         = {test_case}"     + \
            TERMINAL_COLORS.DEFAULT
            )
            print('————————————————————————END_OF_TEST—————————————————————————\n\n')
            return False
    except:
        print(TERMINAL_COLORS.WARNING                   + \
            f"Test {n_of_test} fall. Prog output:\n"    + \
            stdout_data + "'"                           + \
        TERMINAL_COLORS.DEFAULT
        )
        return False


def show_total_test_stat(n_of_test, passed_test, failed):
    print('===========TOTAL==============')
    print(TERMINAL_COLORS.OKBLUE                          + \
            f"Total num of tests = {n_of_test + 1}\n"+ \
            f"Num of passed      = {passed_test}"         + \
    TERMINAL_COLORS.DEFAULT
    )
    if (len(failed)):
        print('FAILED:')
        for failed_test in failed:
            print (TERMINAL_COLORS.ERROR   +\
                   failed_test, "\n"        +\
                   TERMINAL_COLORS.DEFAULT )
    print('==============================')


# -----------------------------------------------------------------------------------------

def find_sha1(sorted_dat, correct):
    pipe = Popen(["sha1sum", sorted_dat, correct], stdout = PIPE)
    stdout_data = pipe.communicate()
    string_data = stdout_data[0].decode()
    lst = string_data.split()
    input_sha = lst[0]
    decomp_sha = lst[2]
    return input_sha, decomp_sha


def run_test(prog_name, args, input_dat):
    my_sorted_file = input_dat.replace(args.input_file, args.sorted_file)
    pipe = Popen([prog_name, "-k", input_dat, "-o" + my_sorted_file], stdout = PIPE)
    stdout_data = pipe.communicate()

    correct_res_file = input_dat.replace(args.input_file, args.answer_file)

    return find_sha1(my_sorted_file, correct_res_file)


def run_test_data(prog_name, args):
    passed_test = 0
    n_test      = 0
    failed = []

    for (n_test, input_file) in zip(range(min(len(data_files_names), args.num_to_test)), data_files_names):
        print('————————————————————————START_OF_TEST———————————————————————')

        prog_res, correct_res = run_test(prog_name, args, input_file)
        if (check_output_data(n_test, prog_res, correct_res, input_file)):
            passed_test += 1
        else:
            failed.append(input_file)

    show_total_test_stat(n_test, passed_test, failed)

# -----------------------------------------------------------------------------------------

def add_parse_arguments(parser):
    parser.add_argument("-n",  "--num_to_test", type = int, default = 1)
    parser.add_argument('-if',  "--input_file",  type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/my_test_dat/')
    parser.add_argument('-sf',  '--sorted_file',          type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/my_test_sort/')
    parser.add_argument('-af', '--answer_file',          type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/my_test_ans/')

# -----------------------------------------------------------------------------------------

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    add_parse_arguments(parser)
    args = parser.parse_args()

    get_files_names(data_files_names, args.input_file)

    run_test_data("./tatlin", args)
