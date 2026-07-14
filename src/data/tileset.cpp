#include "data/tileset.h"

#include <stb/stb_image.h>

namespace viewer {
namespace data {

ImageData::~ImageData()
{
    stbi_image_free(data);
}

} // namespace data
} // namespace viewer
