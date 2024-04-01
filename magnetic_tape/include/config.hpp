#pragma once

#include <fstream>
#include <stdexcept>
#include <vector>
#include <boost/program_options.hpp>

//-----------------------------------------------------------------------------------------

struct set_up_t {
    public:
        std::string input_tape;
        std::string output_tape;
        size_t maxdict = 1000000;

        boost::program_options::options_description set_option_description() {
            using namespace boost::program_options;
            options_description desc{"Options"};
            desc.add_options()
            ("help,h", "help screen")
            ("keep,k",  value<std::string>(), "source_dir")
            ("maxdict", value<int>()->default_value(1000000), "dictionray_dir")
            ("OUTPUT,o",value<std::string>(), "dest_dir");

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

            if (args.count("help")) //TODO: remove here just for debug
                std::cout << desc << '\n';
            if (args.count("keep"))
                src_dir = std::move(args["keep"].as<std::string>());
            else
                throw std::runtime_error("No source dir was mentioned\n");
            if (args.count("train")) {
                if (!args.count("OUTPUT"))
                    throw std::runtime_error("Output file for dict is neccessery param");
                train_mode = true;
            }
            if (args.count("dict")) {
                dictionary_mode = true;
                dict_dir = std::move(args["dict"].as<std::string>());
            }
            if (args.count("maxdict"))
                maxdict = args["maxdict"].as<int>();
            if (args.count("OUTPUT")) {
                dest_dir = std::move(args["OUTPUT"].as<std::string>());
            }
        }

//-----------------------------------------------------------------------------------------

        bool is_train_mode() const {return train_mode;}
        bool is_dict_mode()  const {return dictionary_mode;}
        void write_dictionary_to_file()    const;
        int  get_maxdict() const {return maxdict;}
        const char* get_src_dir()   const {return src_dir.c_str();};
        const char* get_dict_dir() const {
            if (dictionary_mode)
                return dict_dir.c_str();
            else
                throw std::runtime_error("Request to dictionary dir in dict_mode=false\n");
        };
        const char* get_dest_dir() const {
            return dest_dir.c_str();
        };


//-----------------------------------------------------------------------------------------

};
