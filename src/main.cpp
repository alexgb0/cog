#include <iostream>

#define INI_IMPLEMENTATION
#include "ini.hpp"

int main(int argc, char **argv)
{
    auto ini_obj = ini::open_file("cog.ini");
    if (!ini_obj.has_value())
    {
        std::cerr << "FAILED TO OPEN FILE OR PARSE IT\n";
        return 0;
    }

    ini::ini_t hini = ini_obj.value();
    auto res = ini::get_value<std::string>(hini, "general", "foo", "DEFAULT");

    std::cout << "INI VALUE: " << res << "\n";

    return 0;
}
