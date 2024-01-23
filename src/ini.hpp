#pragma once

/*
 * ini.hpp - a simple ini parser
 * by Alex G. B.
 * license MIT
 * -------------------------------------------
 * This header is support only the *bare minimun* for parsing ini.
 */

#ifndef INI_HPP
#define INI_HPP

#include <memory>
#include <unordered_map>
#include <any>
#include <optional>
#include <string>
#include <iostream>
#include <exception>

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


    //DEV
    inline void print_keys(const ini_t& ini)
    {
        for (const auto& [key, value] : ini.data)
        {
            std::cout << "[" << key << "]\n";

            for (const auto& [key2, value2] : value)
                std::cout << "\t" << key2 << " = " << std::any_cast<std::string>(value2) << "\n";
        }
    }

   
    // WHY WE CAN'T HAVE FUNCTION TEMPLATES IN SOURCE FILES??????
    // This funciton is really important because if you don't know how maps works, and you try to do
    // ini["section"]["key"]
    // And it doesn't exist, you would be assigning a new value.
    template<typename T>
    inline T get_value(const ini_t& ini, const std::string& section, const std::string& key, T default_value)
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
            std::cerr << "[INI/WARNING] can't find value " << section << "::" << key << " returning " << default_value << "\n";
            return default_value;
        }
    }
}
#endif // !INI_HPP
