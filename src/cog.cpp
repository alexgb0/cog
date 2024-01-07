#include "cog.hpp"
#include <cstdlib>

#define INI_IMPLEMENTATION
#include "ini.hpp"

int cog::load_ini_default()
{
    // We know that the file exist.
    std::optional<ini::ini_t> optional_ini = ini::open_file("./cog.ini");
    if (!optional_ini.has_value())
    {
        std::cerr << "Error while parsing cog.ini\n";
        return EXIT_FAILURE;
    }

    ini::ini_t hini = optional_ini.value();

    cog::cog_t cog{};
    cog.compailer = ini::get_value<std::string>(hini, "general", "compailer", "g++");
    cog.flags = ini::get_value<std::string>(hini, "general", "flags", "");
    // This will change. We will add sub sections [dir/src] like so, but right now ini lib doesn't have it.
    cog.folders = ini::get_value<std::string>(hini, "general", "folders", "./src");


    return EXIT_SUCCESS;
}
