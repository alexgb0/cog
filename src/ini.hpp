#pragma once
#include <ios>
#include <stdexcept>
#ifndef INI_H
#define INI_H
/*
 * ini.hpp - a single header simple ini parser
 * by Alex G. B.
 * license MIT
 * -------------------------------------------
 * This header is support only the *bare minimun* for parsing ini.
 */

#include <memory>
#include <unordered_map>
#include <any>
#include <optional>
#include <string>

#pragma region DECLARATIONS
namespace ini
{
    using map_t = std::unordered_map<std::string,
                                     std::unordered_map<std::string, std::any>>;

    using opt_map_t = std::unordered_map<std::string,
                                         std::unordered_map<std::string,
                                                            std::unordered_map<std::string, std::any>>>;

    struct ini_t
    {
        map_t data;
        opt_map_t opt;
    };

    /**
     * Opens a file and parses it
     */
    // TODO
    std::optional<ini_t> open_file(const std::string& path);
    std::optional<ini_t> parse(std::ifstream& stream);

    // TODO
    template<typename T>
    T get_value(const ini_t& ini, const std::string& section, const std::string& key, T default_value);
}
#endif /* INI_H */

#pragma endregion DECLARATIONS

#ifdef INI_IMPLEMENTATION
#include <fstream>
#include <sstream>
#include <string_view>
#include <iostream>

std::optional<ini::ini_t> ini::parse(std::ifstream& stream)
{
    ini::ini_t ini{};
    std::string line{};
    std::string opt_current_section{};
    std::string current_section;
    // We could make an int so we can increment.
    // This can be useful for nested sections,
    // but it's not something we need right now.
    bool in_section = false;
    while (std::getline(stream, line))
    {
        std::string_view line_view = line;
        if (line[0] == '[')
        {
            size_t pos_opt = line_view.find("/");
            size_t pos = line_view.find("]");
            // We should account if the user puts [/foo], but we can handle it after
            if (pos_opt > 1 && pos_opt < line_view.size())
            {
                opt_current_section = std::string{line_view.substr(1, pos_opt-1)};
                current_section = std::string{line_view.substr(pos_opt+1,
                                                               (pos_opt-line_view.length())+pos-1)}; // Get the section name
            }
            else
            {
                opt_current_section = "";
                current_section = std::string{line_view.substr(1, pos-1)}; // Get the section name
            }

            in_section = true;
            continue;
        }

       if (in_section)
       {
           // We only support comments in the beggining of the line.
           // Anything else after the equals token will be parsed as
           // the value of the key
           if (line[0] == '#') continue;

           size_t pos = line_view.find("=");
           std::string key{line_view.substr(0, pos)};
           std::string val{line_view.substr(pos+1)};

           if (opt_current_section != "")
               ini.opt[opt_current_section][current_section][key] = std::make_any<std::string>(val);
           else
               ini.data[current_section][key] = std::make_any<std::string>(val);

           in_section = false;
       }
    }

    return ini;
}

std::optional<ini::ini_t> ini::open_file(const std::string& path)
{
    // 1. open the file
    // 2. parse it
    // 3. return the object

    if (path.empty()) return std::nullopt;

    std::ifstream file(path, std::ios_base::in);
    if (file.fail()) return std::nullopt;

    return ini::parse(file);
}


// This funciton is really important because if you don't know how maps works, and you try to do
// ini["section"]["key"]
// And it doesn't exist, you would be assigning a new value.
template<typename T>
T ini::get_value(const ini::ini_t& ini, const std::string& section, const std::string& key, T default_value)
{
    // We could use std::map::contains
    // But it's only for C++20
    // C++20 is too new for some people so we will do this approach
    try
    {
        return std::any_cast<T>(ini.data.at(section).at(key));
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "out_of_range\n";
        return default_value;
    }
}
#endif
