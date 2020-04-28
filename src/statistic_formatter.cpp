#include "statistic_formatter.h"
#include <sstream>

std::string statistic_formatter::format(context_sptr stat)
{
    std::stringstream ss;

    ss << stat->name << " statistic: "
              << stat->num_commands << " commands, "
              << stat->num_blocks << " blocks";
    return ss.str();
}
std::string statistic_formatter::format(const command_handler_statistic& stat)
{
    std::stringstream ss;
    ss << "main thread statistic: "
              << stat.num_lines << " lines, "
              << stat.num_commands << " commands, "
              << stat.num_blocks << " blocks";
    return ss.str();
}
