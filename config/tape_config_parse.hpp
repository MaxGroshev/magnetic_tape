#pragma once

#include <fstream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <string>

//-----------------------------------------------------------------------------------------

namespace fs = std::filesystem;

//-----------------------------------------------------------------------------------------

class tape_config_t {

    using chrono_mcs = typename std::chrono::microseconds;
    private:
        size_t max_ram_size = 8196;
        chrono_mcs rewind_delay {0};
        chrono_mcs move_forward_delay {0};
        chrono_mcs move_backward_delay {0};
        chrono_mcs read_delay {0};
        chrono_mcs write_delay {0};

    public:
        tape_config_t() {};
        tape_config_t(fs::path config_file) {
            std::ifstream config_data;
            config_data.open(config_file);
            if (!config_data.good()) {
                throw std::runtime_error(std::string{"Config file does not exist: " +
                                         std::string{config_file}});
            }

            std::string data {};
            size_t pos = std::string::npos;
            while(std::getline(config_data, data)) {
                if (std::string::npos != (pos = data.find("max_ram_size="))) {
                    std::string str = data.substr(strlen("max_ram_size="));
                    max_ram_size = std::stoll(str);
                }
                else if (std::string::npos != (pos = data.find("rewind_delay="))) {
                    std::string str = data.substr(strlen("rewind_delay="));
                    rewind_delay = std::chrono::microseconds(std::stoll(str));
                }
                else if (std::string::npos != (pos = data.find("move_forward_delay="))) {
                    std::string str = data.substr(strlen("move_forward_delay="));
                    move_forward_delay = std::chrono::microseconds(std::stoll(str));
                }
                else if (std::string::npos != (pos = data.find("move_backward_delay="))) {
                    std::string str = data.substr(strlen("move_backward_delay="));
                    move_backward_delay = std::chrono::microseconds(std::stoll(str));
                }
                else if (std::string::npos != (pos = data.find("read_delay="))) {
                    std::string str = data.substr(strlen("read_delay="));
                    read_delay = std::chrono::microseconds(std::stoll(str));
                }
                else if (std::string::npos != (pos = data.find("write_delay="))) {
                    std::string str = data.substr(strlen("write_delay="));
                    write_delay = std::chrono::microseconds(std::stoll(str));
                }
            }
            config_data.close();
        }

//-----------------------------------------------------------------------------------------

        size_t     get_ram_size()        const {return max_ram_size;};
        chrono_mcs get_rewind_delay()        const {return rewind_delay;};
        chrono_mcs get_move_forward_delay()  const {return move_forward_delay;};
        chrono_mcs get_move_backward_delay() const {return move_backward_delay;};
        chrono_mcs get_read_delay()          const {return read_delay;};
        chrono_mcs get_write_delay()         const {return write_delay;};

        void dump() const {
            std::clog << "Dump_of tape_config:\n" <<
            "Write delay :" << write_delay << std::endl <<
            "Read delay :" << read_delay << std::endl <<
            "Move forward delay :" << move_forward_delay << std::endl <<
            "Move backward delay :" << move_backward_delay << std::endl <<
            "Rewind delay :" << rewind_delay << std::endl;
        }

//-----------------------------------------------------------------------------------------

};
