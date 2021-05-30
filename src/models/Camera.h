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

    class Camera {
    private:
        enum class CameraCommands {
            GENERATE,
            DRAW
        };
        using command_pair = std::pair<Camera::CameraCommands, unsigned int>;
        const static color_t BACKGROUND;
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
         * @param localization
         * @param direction
         * @param up
         * @param viewAngle
         * @param width
         * @param height
         * @param thread - number of threads, if < 0 then uses all available hardware threads
         */
        Camera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up,
               bool use_interlacing = false)
                : Camera(localization, direction, up, -1, use_interlacing) {}

        Camera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up,
               int threads, bool use_interlacing = false)
                : Camera(localization, direction, up, VIEW_ANGLE, WIDTH, HEIGHT, threads, use_interlacing) {}

        Camera(Eigen::Vector3d localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up,
               double viewAngle, int width, int height, int thread, bool use_interlacing);

        ~Camera();

        std::shared_ptr<Scene> getScene() { return _scene; }

        pixel *takePhoto();

        void setCamera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up);

    private:
        void applyCommand(Camera::CameraCommands cmd);

        [[noreturn]] void threadHandler();

        [[nodiscard]] Line generateRay(unsigned int x, unsigned int y) const;

        [[nodiscard]] color_t handleRay(Line& ray, color_t color = BLACK, unsigned int reflection = 0) const;
    };
}


#endif //RAYMARCHING_CAMERA_H
