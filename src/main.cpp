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

    bio::io_context io_context;

    server server(io_context, result->port);

    io_context.run();

    /*
    auto cmd_handler = std::make_shared<command_handler>(result.value());

    auto console_out_subscriber = std::make_shared<subscriber>(1,
            subscriber_handler_creator::create_console_handler());
    cmd_handler->subscribe(console_out_subscriber);

    auto file_out_subscriber = std::make_shared<subscriber>(2,
            subscriber_handler_creator::create_file_handler());
    cmd_handler->subscribe(file_out_subscriber);

    signals_handler::set_state(cmd_handler, {console_out_subscriber, file_out_subscriber});
    std::signal(SIGINT, signals_handler::handle_sigint);

    std::string argument;
    commands_factory cmd_factory;
    while(std::getline(std::cin, argument))
    {
        try {
            auto command = cmd_factory.create_command(argument);
            cmd_handler->add_command(std::move(command));
        }
        catch(const std::logic_error& ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    cmd_handler->stop_handling();

    std::cout << statistic_formatter::format(cmd_handler->statistic()) << std::endl;
    std::cout << statistic_formatter::format(console_out_subscriber->get_worker_context(0)) << std::endl;
    std::cout << statistic_formatter::format(file_out_subscriber->get_worker_context(0)) << std::endl;
    std::cout << statistic_formatter::format(file_out_subscriber->get_worker_context(1)) << std::endl;
*/
    return 0;
}


