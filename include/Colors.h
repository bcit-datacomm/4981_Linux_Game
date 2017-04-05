#ifndef COLORS_H
#define COLORS_H
//Color Data
static constexpr SDL_Color SDL_WHITE_RGB = {255, 255, 255};
static constexpr SDL_Color SDL_RED_RGB = {143, 63, 61};
static constexpr SDL_Color SDL_BLACK_RGB = {0, 0, 0};
static constexpr SDL_Color SDL_GREEN_RGB = {54, 96, 81};

//RGB color information
static constexpr std::array<size_t, 3> WHITE_RGB = {255, 255, 255};
static constexpr std::array<size_t, 3> BLACK_RGB = {0, 0, 0};
static constexpr std::array<size_t, 3> YELLOW_RGB = {148, 139, 77};
static constexpr std::array<size_t, 3> LT_GREEN_RGB = {92, 133, 108};
static constexpr std::array<size_t, 3> BLUE_RGB = {83, 161, 219};
static constexpr std::array<size_t, 3> GREEN_RGB = {0, 255, 0};
static constexpr std::array<size_t, 3> RED_RGB = {255, 0, 0};
static constexpr size_t OPAQUE = 255;
static constexpr size_t MAX_RGB_VALUE = 255;
static constexpr size_t MIN_RGB_VALUE = 0;

#endif
