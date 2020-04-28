#include "util.h"

#include <sstream>

std::string util::prepare_string_command(const commands& cmds)
{
    std::stringstream ss;
    ss << "bulk: ";

    size_t cmds_size = cmds.size();
    for(size_t i = 0; i < cmds_size; ++i)
    {
        ss << cmds[i];
        if(i != cmds_size - 1)
            ss << ", ";
    }

    return ss.str();
}
