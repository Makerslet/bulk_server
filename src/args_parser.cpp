#include "args_parser.h"

#include <iostream>

args_parser::args_parser() :
    _description("allowed options")
{
    _description.add_options()
            ("help", "input arguments description")
            ("port", boost::program_options::value<short>(),
             "set server listen port (required)")
            ("length", boost::program_options::value<int>(),
             "set bulk length (required, value: [1, +65536]");
}

args_parser::optional_result args_parser::parse(int argc, char **argv)
{
    args_result args_value;
    try {
        auto parsed_options = boost::program_options::parse_command_line(argc, argv, _description);
        boost::program_options::store(parsed_options, _values_storage);

        if(_values_storage.count("help"))
        {
            std::cout << _description << std::endl;
            return optional_result();
        }

        if(!_values_storage.count("port"))
            throw std::logic_error("server listen port wasn't set");
        else
        {
            int port = _values_storage["port"].as<short>();
            if(port < 0)
                throw std::logic_error("port can't have a negative value");
            if(port == 0)
                throw std::logic_error("port can't have 0");

            args_value.port = static_cast<unsigned short>(port);
        }

        if(!_values_storage.count("length"))
            throw std::logic_error("bulk length wasn't set");
        else
        {
            int bulk_length = _values_storage["length"].as<int>();
            if(bulk_length < 0)
                throw std::logic_error("bulk length can't have a negative value");
            if(bulk_length == 0)
                throw std::logic_error("bulk length can't have 0");

            args_value.bulk_length = static_cast<size_t>(bulk_length);
        }
    }
    catch(const std::logic_error& ex)
    {
        std::cout << "error: " << ex.what() << std::endl;
        std::cout << _description;
        return optional_result();
    }

    return optional_result(args_value);
}
