#include "cog.hpp"
#include "cmd.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <vector>

#define INI_IMPLEMENTATION
#include "ini.hpp"

namespace fs = std::filesystem;

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

    cog.language = ini::get_value<std::string>(hini, "general", "language", "");
    if (cog.language == "")
    {
        std::cerr << "Error: language not specified.\n";
        return EXIT_FAILURE;
    }

    std::transform(cog.language.begin(), cog.language.end(), cog.language.begin(), [](char c){ return std::tolower(c); });
    std::string default_compailer;
    if (cog.language == "cpp" || cog.language == "c++")
    {
        default_compailer = "g++";
        cog.language = "cpp";
    }
    else if (cog.language == "c")
        default_compailer = "cc";
    else
        default_compailer = "gcc";

    cog.compailer = ini::get_value<std::string>(hini, "general", "compailer", default_compailer);

    cog.flags = ini::get_value<std::string>(hini, "general", "flags", "");
    // This will change. We will add sub sections [dir/src] like so, but right now ini lib doesn't have it.
    cog.folders = ini::get_value<std::string>(hini, "general", "folders", "./src");


    cog::resolve_files(cog, cog.folders);

    return EXIT_SUCCESS;
}


void cog::resolve_files(cog::cog_t& cog, const std::string& dir)
{
    std::vector<std::string> files{};
    //TODO exceptions in case user inputs a folder that doens't exist
    for (const auto& file : fs::recursive_directory_iterator(dir))
    {
        if (!file.is_regular_file()) continue;
        const std::string& ext = file.path().extension().string();

        if (cog.language == "cpp")
        {
            if (ext == ".cpp" || ext == ".cc") // TODO: don't hardcode this
                files.push_back(file.path().string());
        }
        else if (cog.language == "c")
        {
            if (ext == ".c")
                files.push_back(file.path().string());
        }
    }

    cog._files_path = std::move(files);
}


void cog::build_command(cog::cog_t& cog)
{
    cmd::create(cog.compailer, {});
}
