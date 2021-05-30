#ifndef RAYMARCHING_CAMERA_H
#define RAYMARCHING_CAMERA_H

#include <cmath>
#include "memory"
#include "scene.h"
#include "models3.h"
#include "const.h"
#include "algebra.h"
#include "syncqueue.h"
#include "controlnum.h"
#include <thread>
#include <vector>
#include <Eigen/Dense>

namespace RayMarching {

    using pixel = unsigned char;
    /** \brief Class representing camera object
     *
     * Class representing camera object. Controls whole rendering process of the RayMarching lib.
     */
    class Camera {
    protected:
        /**
         * Enum describing Commands available for the Camera threads
         */
        enum class CameraCommands {
            GENERATE, /**< Generate new Camera rays */
            DRAW /**< Draw new image buffer */
        };
        const static color_t BACKGROUND; /**< Default background color */
    private:
        using command_pair = std::pair<Camera::CameraCommands, unsigned int>;
        const double _viewAngle;
        const unsigned int _width;
        const unsigned int _height;
        const unsigned int _awaitFor;
        const unsigned int _jumpDist;
        const unsigned int _totalThreads;
        const bool _interlace;
        bool _terminate = false;
        bool _evenFrame;
        pixel *_buffer;
        Eigen::Vector3d _localization;
        Eigen::Vector3d _forward;
        Eigen::Vector3d _upward;
        Eigen::Vector3d _right;
        Line **_rays;
        std::shared_ptr<Scene> _scene;
        std::vector<std::thread> _drones;
        SyncQueue<command_pair> _queue;
        ControlNum _cn;

    public:
        /**
         * Spawns new camera
         * @param localization Vector describing Camera position point
         * @param direction Vector representing point at which Camera looks at
         * @param up Vector describing where is the *up* for the camera
         * @param use_interlacing Whether to use interlacing (https://en.wikipedia.org/wiki/Interlaced_video)
         */
        Camera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up,
               bool use_interlacing = false)
                : Camera(localization, direction, up, -1, use_interlacing) {}

        /**
         * Spawns new camera with given number of threads
         * @param localization Vector describing Camera position point
         * @param direction Vector representing point at which Camera looks at
         * @param up Vector describing where is the *up* for the camera
         * @param threads number of threads used in the rendering. If threads is negative integer, std::thread::hardware_concurrency is used
         * @param use_interlacing Whether to use interlacing (https://en.wikipedia.org/wiki/Interlaced_video)
         */
        Camera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up,
               int threads, bool use_interlacing = false)
                : Camera(localization, direction, up, VIEW_ANGLE, WIDTH, HEIGHT, threads, use_interlacing) {}

        /**
         *
         * Spawns new camera with given number of threads
         * @param localization Vector describing Camera position point
         * @param direction Vector representing point at which Camera looks at
         * @param up Vector describing where is the *up* for the camera
         * @param viewAngle view angle
         * @param width Canvas width
         * @param height Canvas height
         * @param threads number of threads used in the rendering. If threads is negative integer, std::thread::hardware_concurrency is used
         * @param use_interlacing Whether to use interlacing (https://en.wikipedia.org/wiki/Interlaced_video)
         */
        Camera(Eigen::Vector3d localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up,
               double viewAngle, int width, int height, int thread, bool use_interlacing);

        /**
         * Camera destructor
         */
        ~Camera();

        /**
         * Scene getter
         * @return shared pointer owning Scene object
         */
        std::shared_ptr<Scene> getScene() { return _scene; }

        /**
         * Render frame
         * @return unsigned char[3 * WIDTH * HEIGHT] OpenGL RGB 8bit buffer
         */
        pixel *takePhoto();

        void setCamera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up);

    private:

        [[noreturn]] void threadHandler();

        [[nodiscard]] Line generateRay(unsigned int x, unsigned int y) const;

        [[nodiscard]] color_t handleRay(Line& ray, color_t color = BLACK, unsigned int reflection = 0) const;
    protected:
        void applyCommand(Camera::CameraCommands cmd);
    };
}


#endif //RAYMARCHING_CAMERA_H
