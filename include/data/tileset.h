#ifndef VIEWER_DATA_TILESET_H_
#define VIEWER_DATA_TILESET_H_

#include <cstdint>
#include <string>
#include <vector>

namespace viewer {
namespace data {

/**
 * @brief Base grayscale values used by tileset images.
 */
const std::vector<uint8_t> grayscale_palette {
    255, 238, 222, 205, 189, 172, 156, 139, 115, 98, 82, 65, 49, 32, 16, 0
};

/**
 * @brief Basic image data struct.
 */
struct ImageData
{
    ~ImageData();

    int width;
    int height;
    int channels;

    unsigned char* data { nullptr };
};

/**
 * @brief Stores a tiles index and flags.
 */
struct Tile
{
    uint8_t index;
    uint8_t flags; // I'll decode what each flag means later
};

/**
 * @brief Stores metatile data.
 */
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

/**
 * @brief Stores tileset data.
 */
struct Tileset
{
    ImageData image;

    std::vector<Metatile> metatiles;
    std::vector<Palette> palettes;
};

} // namespace data
} // namespace viewer

#endif // VIEWER_DATA_TILESET_H_