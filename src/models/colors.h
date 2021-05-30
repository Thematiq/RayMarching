#ifndef RAYMARCHING_COLORS_H
#define RAYMARCHING_COLORS_H


namespace RayMarching {
    using pixel = unsigned char;

    typedef struct color {
        pixel R;
        pixel G;
        pixel B;
        [[nodiscard]] struct color mix(const struct color &other) const {
            return {
                static_cast<pixel>((R + other.R) / 2),
                static_cast<pixel>((G + other.G) / 2),
                static_cast<pixel>((B + other.B) / 2)
            };
        }
    } color_t;

    constexpr color_t WHITE = {0xff, 0xff, 0xff};
    constexpr color_t GRAY    = {0xbf, 0xbf, 0xbf};
    constexpr color_t BLACK = {0x00, 0x00, 0x00};
    constexpr color_t RED = {0xff, 0x00, 0x00};
    constexpr color_t GREEN = {0x00, 0xff, 0x00};
    constexpr color_t BLUE = {0x00, 0x00, 0xff};
}


#endif //RAYMARCHING_COLORS_H
