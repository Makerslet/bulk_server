#include "commands_factory.h"
#include "command_handler.h"
#include "subscriber.h"
#include "args_parser.h"
#include "subscriber_handler_creator.h"
#include "statistic_formatter.h"
#include "signals_handler.h"
#include "async_server.h"

#include <csignal>
#include <iostream>

/**
 * @brief Entry point
 *
 * Execution of the program
 * starts here.
 *
 * @return Program exit status
 */

int main (int argc, char** argv)
{
    args_parser parser;
    auto result = parser.parse(argc, argv);
    if(!result.has_value())
        return 0;


    auto cmd_handler = std::make_shared<command_handler>(result->bulk_length);

    auto console_out_subscriber = std::make_shared<subscriber>(1,
            subscriber_handler_creator::create_console_handler());
    cmd_handler->subscribe(console_out_subscriber);

    auto file_out_subscriber = std::make_shared<subscriber>(1,
            subscriber_handler_creator::create_file_handler());
    cmd_handler->subscribe(file_out_subscriber);

    bio::io_context io_context;
    server server(io_context, result->port);
    io_context.run();

    return 0;
}


