#ifndef MAP_VALUE_H
#define MAP_VALUE_H

// Purpose of this file was to separate map value constants from being defined in source files.
// Globally useable map dimension values.

namespace MapValues {
    // Tile Size
    constexpr int TILE_SIZE = 150;

    // Map width/height
    constexpr int MAP_WIDTH = 100 * 150;
    constexpr int MAP_HEIGHT = 100 * 150;

    // Base width/height
    constexpr int BASE_WIDTH = 7 * 150;
    constexpr int BASE_HEIGHT = 7 * 150;
}
#endif
