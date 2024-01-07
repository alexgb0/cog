#include "cmd.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <cstdlib>
#include <iostream>

cmd::cmd_t cmd::create(std::string command, std::vector<std::string> args)
{
    cmd::cmd_t ret{};
    ret.commands.push_back(command + " ");
    for (const auto& arg : args)
    {
        ret.commands.push_back(arg);
        ret.commands.push_back(" ");
    }

    return ret;
}

bool cmd::execute(cmd::cmd_t cmd)
{
    std::string command = std::accumulate(cmd.commands.begin(), cmd.commands.end(), std::string{});
    std::cout << "[COG] " << command << "\n";
    return std::system(command.c_str());
}
