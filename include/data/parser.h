#ifndef VIEWER_DATA_PARSER_H_
#define VIEWER_DATA_PARSER_H_

#include <filesystem>
#include <string>
#include <vector>

#include "data/tileset.h"

namespace viewer {
namespace data {

/**
 * @brief Static function responsible for parsing tileset data.
 */
class Parser
{
public:
    static Tileset parse_tileset(const std::string& name);

private:
    static std::vector<Metatile> parse_metatiles(const std::filesystem::path& tileset_path);
    static Metatile parse_metatile();

    static std::vector<Palette> parse_palettes(const std::filesystem::path& tileset_path);
    static Palette parse_palette();
};

}
}

#endif // VIEWER_DATA_PARSER_H_
