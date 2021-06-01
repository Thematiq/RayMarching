#ifndef RAYMARCHING_COLORS_H
#define RAYMARCHING_COLORS_H


namespace RayMarching {
    using pixel = unsigned char;

    /** \brief 3 byte color representation structure
     *
     */
    typedef struct color {
        pixel R;        /**< Red color */
        pixel G;        /**< Green color */
        pixel B;        /**< Blue color */

        /**
         * Creates arithmetic average of two colors
         * @param other second color
         * @return mix of colors
         */
        [[nodiscard]] struct color mix(const struct color &other) const {
            return {
                static_cast<pixel>((R + other.R) / 2),
                static_cast<pixel>((G + other.G) / 2),
                static_cast<pixel>((B + other.B) / 2)
            };
        }
    } color_t;

    constexpr color_t WHITE = {0xff, 0xff, 0xff};       /**< White color macro */
    constexpr color_t GRAY    = {0xbf, 0xbf, 0xbf};     /**< Gray color macro */
    constexpr color_t BLACK = {0x00, 0x00, 0x00};       /**< Black color macro */
    constexpr color_t RED = {0xff, 0x00, 0x00};         /**< Red color macro */
    constexpr color_t GREEN = {0x00, 0xff, 0x00};       /**< Green color macro */
    constexpr color_t BLUE = {0x00, 0x00, 0xff};        /**< Blue color macro */
}


#endif //RAYMARCHING_COLORS_H
