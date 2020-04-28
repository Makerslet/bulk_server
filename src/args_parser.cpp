#include "args_parser.h"

#include <iostream>

args_parser::args_parser() :
    _description("allowed options")
{
    _description.add_options()
            ("help", "Type exit for exit")
            ("length", boost::program_options::value<int>(),
             "set bulk length (required, value: [1,+65536])");
}

std::optional<size_t> args_parser::parse(int argc, char **argv)
{
    std::optional<size_t> return_value;
    try {
        auto parsed_options = boost::program_options::parse_command_line(argc, argv, _description);
        boost::program_options::store(parsed_options, _values_storage);

        if(_values_storage.count("help"))
        {
            std::cout << _description << std::endl;
            return return_value;
        }

        if(!_values_storage.count("length"))
            throw std::logic_error("bulk length wasn't set");
        else
        {
            int tmp = _values_storage["length"].as<int>();
            if(tmp < 0)
                throw std::logic_error("bulk length can't have a negative value");
            if(tmp == 0)
                throw std::logic_error("bulk length can't have 0");

            return_value = static_cast<size_t>(tmp);
        }
    }
    catch(const std::logic_error& ex)
    {
        std::cout << "error: " << ex.what() << std::endl;
        std::cout << _description;
    }

    return return_value;
}
