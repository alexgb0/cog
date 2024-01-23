#include "ini.hpp"

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

