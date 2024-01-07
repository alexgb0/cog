#pragma once

#include <vector>
#include <string>
#include "cmd.hpp"

namespace cog
{

    struct cog_t
    {
        std::string language;
        std::string compailer;
        std::string flags;
        std::string libs;
        std::string folders;
        std::string output_file;
        std::string output_folder;

        std::vector<std::string> _files_path;

        cmd::cmd_t command;
    };


    // UTIL
    void resolve_files(cog_t& cog, const std::string& dir);

    // Prepares the output. Making the folder if necessary.
    void create_output(cog_t& cog);

    void build_command(cog_t& cog);
    bool run_command(cog_t& cog);

    /**
     * Main function of cog, this function loads the file from whatever has been called.
     */
    int load_ini_default();
}
