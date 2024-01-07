#pragma once

#include <string>

#define INI_IMPLEMENTATION
#include "ini.hpp"

namespace cog
{

    struct cog_t
    {
        std::string compailer;
        std::string flags;
        std::string libs;
        std::string folders;
    };


    /**
     * Main function of cog, this function loads the file from whatever has been called.
     */
    int load_ini_default();
}
