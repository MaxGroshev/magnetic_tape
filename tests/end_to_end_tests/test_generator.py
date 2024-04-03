import subprocess
import argparse
import random
import numpy as np
import os
import sys

# -----------------------------------------------------------------------------------------

def print_test_data(args, test_data):


    with open(args.data_file, "wb") as file:
        for num in test_data:
            file.write(num.to_bytes(4, byteorder='little', signed=True))

    test_data.sort()

    with open(args.answer_file, "wb") as file:
        for num in test_data:
            file.write(num.to_bytes(4, byteorder='little', signed=True))


def generate_test_data(args):
    test_data = []

    for i in range(args.count):
        test_data.append(random.randint(args.diaposon[0], args.diaposon[1]))

    return test_data

# -----------------------------------------------------------------------------------------

def add_parse_arguments(parser):
    parser.add_argument("-c",  "--count",         type = int, default = 2048)
    parser.add_argument('-d',  "--diaposon",      nargs = 2, type = int,
                                                  default = [-10000000, 10000000])
    parser.add_argument('-df',  '--data_file',    type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/my_test_dat/gen_test.dat')
    parser.add_argument('-af',  '--answer_file',          type = str, default =
                    os.path.dirname(os.path.abspath(__file__)) + '/my_test_ans/gen_test.dat')

# -----------------------------------------------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    add_parse_arguments(parser)
    args = parser.parse_args()
    # check_args(args)

    test_data = generate_test_data(args)
    print_test_data(args, test_data)
