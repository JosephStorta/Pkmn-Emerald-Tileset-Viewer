#include <logger/logger.h>

#include "data/parser.h"

SET_LOG_MODULE("MAIN");

// Test value for loading tilesets
const std::string tileset_name { "general" };

int main()
{
    LOG_INFO("Running test parse...");
    viewer::data::Parser::parse_tileset(tileset_name);

    return 0;
}
