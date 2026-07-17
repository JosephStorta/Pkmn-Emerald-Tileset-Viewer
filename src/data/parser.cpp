#include "data/parser.h"

#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <memory>
#include <string>

#include <logger/logger.h>

#include <stb/stb_image.h>

#include "data/tileset.h"

namespace viewer {
namespace data {

SET_LOG_MODULE("DATA");

/**
 * @brief Parses tileset data into a Tileset object.
 * @param name The name of the tileset.
 * @return The parsed tileset data.
 */
Tileset* Parser::parse_tileset(const std::filesystem::path& tileset_path)
{
    LOG_INFO( std::format("Parsing tileset: {}", tileset_path.string()) );

    if (!std::filesystem::exists(tileset_path))
    {
        LOG_ERROR("Tileset does not exist");
        return nullptr;
    }

    std::unique_ptr<Tileset> tileset { std::make_unique<Tileset>() };
    tileset->image = *parse_image(tileset_path);
    tileset->metatiles = parse_metatiles(tileset_path);
    tileset->palettes = parse_palettes(tileset_path);

    return tileset.release();
}

/**
 * @brief Parses the image data for the given tileset.
 * @param tileset_path The folder containing the tileset data.
 * @return The parsed image data.
 */
ImageData* Parser::parse_image(const std::filesystem::path& tileset_path)
{
    std::filesystem::path image_path { tileset_path / "tiles.png" };

    LOG_DEBUG( std::format("Parsing image file: {}", image_path.string()) );

    if (!std::filesystem::exists(image_path))
    {
        LOG_ERROR("Tileset image does not exist");
        return {};
    }

    std::unique_ptr<ImageData> image_data { std::make_unique<ImageData>() };
    image_data->data = stbi_load(
        image_path.string().c_str(),
        &image_data->width,
        &image_data->height,
        &image_data->channels,
        3
    );

    return image_data.release();
}

/**
 * @brief Parses the metatile data for the given tileset.
 * @param tileset_path The folder containing the tileset data.
 * @return The parsed metatiles.
 */
std::vector<Metatile> Parser::parse_metatiles(const std::filesystem::path& tileset_path)
{
    std::filesystem::path metatile_path { tileset_path / "metatiles.bin" };

    LOG_DEBUG( std::format("Parsing metatile data: {}", metatile_path.string()) );

    if (!std::filesystem::exists(metatile_path))
    {
        LOG_ERROR("Metatile data does not exist:");
        return {};
    }

    std::ifstream file(metatile_path, std::ios::binary);
    if (!file.is_open())
    {
        LOG_ERROR("Failed to open file");
        return {};
    }

    std::vector<Metatile> metatiles;

    while (!file.eof())
    {
        // Read the file in 16-byte increments
        char data[16];
        file.read(data, 16);

        Metatile metatile { parse_metatile(data) };
        metatiles.push_back(metatile);
    }

    file.close();

    std::filesystem::path attribute_path { tileset_path / "metatile_attributes.bin" };

    LOG_DEBUG( std::format("Parsing metatile attributes: {}", attribute_path.string()) );

    if (!std::filesystem::exists(attribute_path))
    {
        LOG_ERROR("Metatile attribute data does not exist:");
        return metatiles;
    }

    file.open(attribute_path, std::ios::binary);
    if (!file.is_open())
    {
        LOG_ERROR("Failed to open file");
        return metatiles;
    }

    int index { 0 };
    while (!file.eof())
    {
        // Read the file in 2-byte increments
        char data[2];
        file.read(data, 2);

        metatiles[index].attributes = (data[0] << 8) | data[1];
        index++;
    }

    file.close();

    return metatiles;
}

/**
 * @brief Parses a single metatile from binary data.
 * @param data A byte array containing the metatile data.
 * @return The parsed metatile.
 */
Metatile Parser::parse_metatile(char data[])
{
    std::vector<Tile> tiles;
    for (int i = 0; i < 16; i += 2) // Read in 2-byte increments
    {
        int attributes { (int)data[i + 1] };

        tiles.push_back(
            Tile {
                (uint8_t)data[i],
                (uint8_t)( attributes >> 4 ), // First nibble is the palette index

                // Last 4 bits are different flags
                (bool)( attributes & 0b00001000 ), // Secondary tileset check
                (bool)( attributes & 0b00000100 ), // Horizontal flip
                (bool)( attributes & 0b00000010 ), // Vertical flip
                (bool)( attributes & 0b00000001 )  // Bank swap (each tileset can have 2 banks of 256 tiles)
            }
        );
    }

    return Metatile {
        std::vector<Tile>(tiles.begin(), tiles.begin() + 4),
        std::vector<Tile>(tiles.begin() + 4, tiles.end()),
        0x0000
    };
}

/**
 * @brief Parses the palette data for the given tileset.
 * @param tileset_path The folder containing the tileset data.
 * @return The parsed palettes.
 */
std::vector<Palette> Parser::parse_palettes(const std::filesystem::path& tileset_path)
{
    std::filesystem::path palette_path { tileset_path / "palettes" };

    LOG_DEBUG( std::format("Parsing palettes: {}", palette_path.string()) );

    if (!std::filesystem::exists(palette_path))
    {
        LOG_ERROR("Palettes do not exist");
        return {};
    }

    std::vector<Palette> palettes;

    for (const std::filesystem::path& palette_file : std::filesystem::directory_iterator(palette_path))
    {
        palettes.push_back(
            parse_palette(palette_file)
        );
    }

    return palettes;
}

/**
 * @brief Parses the given palette file.
 * @param palette_file The palette file to parse.
 * @return The parsed palette.
 */
Palette Parser::parse_palette(const std::filesystem::path& palette_file)
{
    LOG_DEBUG( std::format("Parsing palette file: {}", palette_file.string()) );

    if (!std::filesystem::exists(palette_file))
    {
        LOG_ERROR("Palette does not exist:");
        return {};
    }

    std::ifstream file(palette_file);
    if (!file.is_open())
    {
        LOG_ERROR("Failed to open file");
        return {};
    }

    std::vector<Color> colors;

    // The firts 3 lines of data are unneeded
    std::string data;
    file >> data; // Palette name
    file >> data; // Metadata
    file >> data; // Palette length (always 16)

    for (int i = 0; i < 16; i++)
    {
        int r;
        int g;
        int b;

        file >> r;
        file >> g;
        file >> b;

        colors.push_back(
            Color {
                static_cast<uint8_t>(r),
                static_cast<uint8_t>(g),
                static_cast<uint8_t>(b)
            }
        );
    }

    file.close();

    return Palette { colors };
}

} // namespace data
} // namespace viewer
