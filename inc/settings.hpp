#ifndef SETTINGS_HPP
#define SETTINGS_HPP

constexpr int WINDOW_WIDTH = 1820;
constexpr int WINDOW_HEIGHT = 1080;
constexpr const char* WINDOW_TITLE = "OpenGL - Water";
constexpr const char* SKYBOX_FACES[6] = {
    "resources/right.jpg",  "resources/left.jpg",  "resources/top.jpg",
    "resources/bottom.jpg", "resources/front.jpg", "resources/back.jpg"};

constexpr float CAMERA_FOV = 45.0f;
constexpr float CAMERA_NEAR = 0.1f;
constexpr float CAMERA_FAR = 2000.0f;

#endif
