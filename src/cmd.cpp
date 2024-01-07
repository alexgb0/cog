#include "cmd.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <cstdlib>

cmd::cmd_t cmd::create(std::string command, std::vector<std::string> args)
{
    cmd::cmd_t ret{};
    ret.commands.reserve(args.size() + 1);
    ret.commands.push_back(command);
    std::copy(args.begin(), args.end(), ret.commands.begin());

    return ret;
}

bool cmd::execute(cmd::cmd_t cmd)
{
    std::string command = std::accumulate(cmd.commands.begin(), cmd.commands.end(), std::string{});
    return std::system(command.c_str());
}
