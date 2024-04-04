#pragma once

#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <boost/program_options.hpp>

//-----------------------------------------------------------------------------------------

namespace fs = std::filesystem;

//-----------------------------------------------------------------------------------------

struct set_up_t final {
    public:
        fs::path input_tape_file;
        fs::path output_tape_file;
        fs::path config_file = "../config/tape_config.conf";

        boost::program_options::options_description set_option_description() {
            using namespace boost::program_options;
            options_description desc{"Options"};
            desc.add_options()
            ("help,h", "help screen")
            ("keep,k",  value<std::string>(), "source_dir")
            ("config-file,cf",  value<std::string>(), "configuration file")
            ("output,o",value<std::string>(), "dest_dir");

            return desc;
        }

    public:
        set_up_t() {};
        set_up_t(const int argc, char** argv) {
            using namespace boost::program_options;

            options_description desc = set_option_description() ;

            variables_map args;
            store(parse_command_line(argc, argv, desc), args);
            notify(args);

            if (args.count("help"))
                std::cout << desc << '\n';
            if (args.count("config-file"))
                config_file = std::move(args["config-file"].as<std::string>());
            if (args.count("keep"))
                input_tape_file = std::move(args["keep"].as<std::string>());
            else
                throw std::runtime_error("No source tape was mentioned\n");
            if (args.count("output"))
                output_tape_file = std::move(args["output"].as<std::string>());
            else
                throw std::runtime_error("No destination tape was mentioned\n");
        }

//-----------------------------------------------------------------------------------------

        fs::path get_input_tape_file()  const {return input_tape_file;};
        fs::path get_output_tape_file() const {return output_tape_file;};
        fs::path get_config_file()      const {return config_file;};

//-----------------------------------------------------------------------------------------

};
