#include "args.hpp"

args::args_t args::make_args(int argc, char **argv)
{
    args_t arg{};
    arg.args.reserve(argc);

    arg.program_name = std::string(argv[1]);
    for (int i = 1; i < argc; i++)
    {
       arg.args[i] = std::string(argv[i]);
    }

    return arg;
}
