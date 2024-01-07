#pragma once

#include <vector>
#include <string>

namespace cmd
{
    struct cmd_t
    {
        std::vector<std::string> commands;
    };

    cmd_t create(std::string command, std::vector<std::string> args);
    bool execute(cmd_t cmd);
}
