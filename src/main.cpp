#include <any>
#include <cstdlib>
#include <iostream>
#include <filesystem>

#include "cog.hpp"


constexpr const char* COG_FILE = "cog.ini";

int main_args(int argc, char** argv) { std::cerr << "NOT IMPLEMENTED\n"; return EXIT_FAILURE; }

int main(int argc, char** argv)
{
    if (argc > 1)
       return main_args(argc, argv);
    // If the user doens't provide any arguments we will get the `cog.ini` in the root dir.
    // If the folder doesn't contain any `cog.ini` then we will rise an error.

    std::filesystem::path file{COG_FILE};
    if (!std::filesystem::exists(file))
    {
        std::cerr << "File `" << COG_FILE << "` not found.\n"
            << "Try to run this command in the the root directory where `" << COG_FILE << "` is.";
        return EXIT_FAILURE;
    }


    return cog::load_ini_default();


    // auto hini = ini::open_file("test.ini");
    // if (!hini.has_value())
    //     std::cerr << "ERROR: can't open the file\n";
    // auto data = hini.value().opt.at("test").at("asd").at("bar");
    // std::cout << "DATA: " << std::any_cast<std::string>(data) << "\n";
    return EXIT_SUCCESS;
}
