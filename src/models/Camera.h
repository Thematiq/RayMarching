#ifndef RAYMARCHING_CAMERA_H
#define RAYMARCHING_CAMERA_H

#include <cmath>
#include "memory"
#include "scene.h"
#include "models3.h"
#include "const.h"
#include "algebra.h"
#include <thread>
#include <vector>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <Eigen/Dense>

using pixel = unsigned char;

class Camera {
private:
    const static color_t BACKGROUND;
    const double _viewAngle;
    const int _width;
    const int _height;
    const bool _interlacing;
    const unsigned int _totalThreads;
    bool _terminate = false;
    pixel* _buffer;
    Eigen::Vector3d _localization;
    Eigen::Vector3d _forward;
    Eigen::Vector3d _upward;
    Eigen::Vector3d _right;
    Line** _rays;
    std::vector<std::thread> _controllerInit;
    std::vector<std::thread> _controllerDraw;
    std::condition_variable _caller;
    std::mutex _callerMutex;
    std::condition_variable _returner;
    std::mutex _pickup;
    std::shared_ptr<Scene> _scene;
    std::condition_variable _queueInitCond;
    std::condition_variable _queueDrawCond;
    std::mutex _queueMutex;
    std::queue<unsigned int> _freeRows;
    std::atomic<unsigned int> _threadsReady;
    std::atomic<int> _rowsDrawed;
    std::atomic<bool> _execute;

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
    Camera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up, bool use_interlacing = false)
    : Camera(localization, direction, up, -1, use_interlacing) {}
    Camera(const Eigen::Vector3d &localization, const Eigen::Vector3d &direction, const Eigen::Vector3d &up, int threads, bool use_interlacing = false)
    : Camera(localization, direction, up, VIEW_ANGLE, WIDTH, HEIGHT, threads, use_interlacing) {}
    Camera(Eigen::Vector3d localization, const Eigen::Vector3d& direction, Eigen::Vector3d up, double viewAngle, int width, int height, int thread, bool use_interlacing);
    ~Camera();
    std::shared_ptr<Scene> getScene(){ return _scene;}
    pixel* takePhoto();

private:
    [[noreturn]] void drawingHandler();
    [[noreturn]] void initHandler();
    [[nodiscard]] Line generateRay(unsigned int x, unsigned int y) const;
    [[nodiscard]] color_t handleRay(unsigned int x, unsigned int y) const;
};



#endif //RAYMARCHING_CAMERA_H
