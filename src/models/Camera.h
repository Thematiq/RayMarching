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
#include "settings.h"
#include <thread>
#include <utility>
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
        const Settings_t _settings;
        const unsigned int _awaitFor;
        const unsigned int _jumpDist;
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
         * Creates new Camera with default settings
         * @param localization Vector3d representing Camera position in the space
         * @param direction Vector3d representing Point at which Camera looks
         * @param up Vector3d representing normal vector for Camera horizontal plane
         */
        Camera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up)
            : Camera(localization, direction, up, Settings_t()) {}

        /**
         * Creates new Camera object
         * @param localization Vector3d representing Camera position in the space
         * @param direction Vector3d representing Point at which Camera looks
         * @param up Vector3d representing normal vector for Camera horizontal plane
         * @param set Settings_t structure defining parameters
         */
        Camera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up, Settings_t set);

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

        /**
         * Moves Camera to a new position
         * @param localization Vector3d representing Camera position in the space
         * @param direction Vector3d representing Point at which Camera looks
         * @param up Vector3d representing normal vector for Camera horizontal plane
         */
        void setCamera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up);

    private:

        /**
         * Function handling _drones threads work
         */
        [[noreturn]] void threadHandler();

        /**
         * Generates new Line based on a pixel position
         * @param x X pixel coordinate (left to right)
         * @param y Y pixel coordinate (top to bottom
         * @return Line representing pixel's ray
         */
        [[nodiscard]] Line generateRay(unsigned int x, unsigned int y) const;

        /**
         * Generates pixel's color based on a provided ray. Additional parameters are used for the recursion
         * @param ray Pixel's Line
         * @param color Last reflection color (used for recursion)
         * @param reflection No reflection (used for recursion)
         * @return Color for a given ray
         */
        [[nodiscard]] color_t handleRay(Line& ray, color_t color = BLACK, unsigned int reflection = 0) const;

        inline
        void applyColor(const unsigned int &x, const unsigned int &y, const color_t &c);
    protected:
        /**
         * Sends command to the render threads and awaits for the synchronization
         * @param cmd CameraCommands command
         */
        void applyCommand(Camera::CameraCommands cmd);
    };
}


#endif //RAYMARCHING_CAMERA_H
