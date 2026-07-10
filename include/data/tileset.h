#ifndef VIEWER_DATA_TILESET_H_
#define VIEWER_DATA_TILESET_H_

#include <cstdint>
#include <string>
#include <vector>

namespace viewer {
namespace data {

struct Tile
{
    uint8_t index;

    uint8_t flags; // I'll decode what each flag means later
};

struct Metatile
{
    // Each metatile is made up of 2 layers of 4 tiles
    std::vector<Tile> background;
    std::vector<Tile> foreground;

    uint16_t attributes; // 16-bit value who's purpose is currently unknown
};

/**
 * @brief Basic RGB color struct.
 */
struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

/**
 * @brief Simple palette of 16 colors.
 */
struct Palette
{
    std::vector<Color> colors;
};

struct Tileset
{
    // TODO: store tileset image data
    std::vector<Metatile> metatiles;
    std::vector<Palette> palettes;
};

} // namespace data
} // namespace viewer

#endif // VIEWER_DATA_TILESET_H_