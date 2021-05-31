#ifndef RAYMARCHING_CONST_H
#define RAYMARCHING_CONST_H

/*!
 * \addtogroup RayMarching
 * @{
 */

//! Namespace containing all tools implemented within RayMarching package
namespace RayMarching {

    constexpr unsigned int WIDTH = 1280;
    constexpr unsigned int HEIGHT = 720;

    constexpr double VIEW_ANGLE = 80;
    constexpr double EPSILON = 0.001;
    constexpr double MAX_DISTANCE = 5;
    constexpr unsigned int MAX_STEPS = 64;
    constexpr unsigned int MAX_REFLECTIONS = 5;
}
/*! @} End of Doxygen Groups */

#endif //RAYMARCHING_CONST_H
