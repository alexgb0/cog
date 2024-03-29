#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <numeric>
#include <vector>
#include <iostream>

#include "ini.hpp"
#include "cog.hpp"
#include "cmd.hpp"

namespace fs = std::filesystem;


int parse_compailer(cog::cog_t& cog, ini::ini_t& ini);
void parse_elements(cog::cog_t& cog, ini::ini_t& ini);

//TODO: clean this function, it does too much
int cog::load_ini_default()
{
    // We know that the file exist.

    cog::cog_t cog{};

    if (cog::parse_ini(cog) == EXIT_FAILURE)
        return EXIT_FAILURE;
    cog::resolve_files(cog, cog.folders);

    return cog::run_command(cog);
}


int cog::parse_ini(cog::cog_t& cog)
{
    std::optional<ini::ini_t> optional_ini = ini::open_file("./cog.ini");
    if (!optional_ini.has_value())
    {
        std::cerr << "Error while parsing cog.ini\n";
        return EXIT_FAILURE;
    }

    ini::ini_t hini = optional_ini.value();
    

    ini::print_keys(hini);

    if (parse_compailer(cog, hini) == EXIT_FAILURE)
        return EXIT_FAILURE;
    parse_elements(cog, hini);

    return EXIT_SUCCESS;
}

void parse_elements(cog::cog_t& cog, ini::ini_t& ini)
{
    cog.flags = ini::get_value<std::string>(ini, "general", "flags", "");
    // This will change. We will add sub sections [dir/src] like so, but right now ini lib doesn't have it.
    cog.folders = ini::get_value<std::string>(ini, "general", "folders", "./src");

    cog.output_file = ini::get_value<std::string>(ini, "general", "output_file", "main");
    cog.output_folder = ini::get_value<std::string>(ini, "general", "output_folder", "out");
}

int parse_compailer(cog::cog_t& cog, ini::ini_t& ini)
{
    cog.language = ini::get_value<std::string>(ini, "general", "language", "");
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

    cog.compailer = ini::get_value<std::string>(ini, "general", "compailer", default_compailer);

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

void cog::create_output(cog::cog_t& cog)
{
    if (fs::exists("./" + cog.output_folder))
        return;
    fs::create_directory("./" + cog.output_folder);
}

void cog::build_command(cog::cog_t& cog)
{
    std::string output_path = fs::path(cog.output_folder).append(cog.output_file).string();
    std::string files_str{};
    for (const auto& f : cog._files_path)
    {
        files_str.append(f);
        files_str.append(" ");
    }

    cog.command = cmd::create(cog.compailer, {
        cog.flags,
        cog.libs,
        "-o " + output_path,
        files_str
    });
}


bool cog::run_command(cog::cog_t& cog)
{
    cog::create_output(cog);
    cog::build_command(cog);
    return cmd::execute(cog.command);
}
