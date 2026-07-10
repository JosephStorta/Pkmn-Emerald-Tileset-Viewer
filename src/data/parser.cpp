#include "data/parser.h"

#include <filesystem>
#include <format>
#include <string>

#include <logger/logger.h>

#include "data/tileset.h"

namespace viewer {
namespace data {

SET_LOG_MODULE("DATA");

// Root directory for tileset data (value subject to change)
const std::filesystem::path tileset_root_path { "./res/primary" };

/**
 * @brief Parses tileset data into a Tileset object.
 * @param name The name of the tileset.
 * @return The parsed tileset data.
 */
Tileset Parser::parse_tileset(const std::string& name)
{
    std::filesystem::path tileset_path { tileset_root_path / name };

    LOG_INFO( std::format("Parsing tileset: ", tileset_path.string()) );

    if (!std::filesystem::exists(tileset_path))
    {
        LOG_ERROR("Tileset does not exist");
        return Tileset{};
    }

    return Tileset
    {
        parse_metatiles(tileset_path),
        parse_palettes(tileset_path)
    };
}

std::vector<Metatile> Parser::parse_metatiles(const std::filesystem::path& tileset_path)
{
    return {};
}

Metatile Parser::parse_metatile()
{
    return {};
}

std::vector<Palette> Parser::parse_palettes(const std::filesystem::path& tileset_path)
{
    return {};
}

Palette Parser::parse_palette()
{
    return {};
}

}
}
