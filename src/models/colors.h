#ifndef RAYMARCHING_COLORS_H
#define RAYMARCHING_COLORS_H

using pixel = unsigned char;

typedef struct {
    pixel R;
    pixel G;
    pixel B;
} color_t;

constexpr color_t WHITE   = {0xff, 0xff, 0xff};
constexpr color_t GRAY    = {0xbf, 0xbf, 0xbf};
constexpr color_t BLACK   = {0x00, 0x00, 0x00};
constexpr color_t RED     = {0xff, 0x00, 0x00};
constexpr color_t GREEN   = {0x00, 0xff, 0x00};
constexpr color_t BLUE    = {0x00, 0x00, 0xff};

#endif //RAYMARCHING_COLORS_H
