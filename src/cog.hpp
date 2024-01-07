#pragma once

#include <vector>
#include <string>

namespace cog
{

    struct cog_t
    {
        std::string language;
        std::string compailer;
        std::string flags;
        std::string libs;
        std::string folders;

        std::vector<std::string> _files_path;
    };


    // UTIL
    void resolve_files(cog_t& cog, const std::string& dir);

    /**
     * Main function of cog, this function loads the file from whatever has been called.
     */
    int load_ini_default();
}
