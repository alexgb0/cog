#pragma once

#include <vector>
#include <string>

namespace args
{
    struct args_t
    {
        std::string program_name;
        std::vector<std::string> args;
    };

    args_t make_args(int argc, char** argv);
}
