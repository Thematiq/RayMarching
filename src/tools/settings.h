#ifndef RAYMARCHING_SETTINGS_H
#define RAYMARCHING_SETTINGS_H

#include <thread>
#include <const.h>

namespace RayMarching {
    /** \brief Container for const Camera Settings
     *
     *  Structure used for defining Camera settings in one place. Create default settings
     *  and then customize for your needs
     */
    typedef struct Settings {
        // General
        unsigned int width;             /**< Canvas width */
        unsigned int height;            /**< Canvas height */
        double viewAngle;               /**< Camera viewAngle */
        // Technical
        unsigned int threads;           /**< Camera rendering threads */
        bool interlace;                 /**< Use interlacing (https://en.wikipedia.org/wiki/Interlaced_video) */
        // Quality
        unsigned int maxReflections;    /**< Maximum number of SDFObject reflection (use 0 for no reflections)*/
        unsigned int maxSteps;          /**< Maximum number of Line steps */
        double maxDistance;             /**< Maximum rendering distance */
        double epsilon;                 /**< Maximum distance between object and the Camera ray */
        /**
         * Default Settings constructor based on a const.h expressions
         */
        Settings()
            : width(WIDTH), height(HEIGHT), viewAngle(VIEW_ANGLE),
              threads(std::thread::hardware_concurrency()), interlace(true),
              maxReflections(MAX_REFLECTIONS), maxSteps(MAX_STEPS), maxDistance(MAX_DISTANCE),
              epsilon(EPSILON) {};
    } Settings_t;
}

#endif //RAYMARCHING_SETTINGS_H
