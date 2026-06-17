#ifndef SETTINGS_HPP
#define SETTINGS_HPP

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr const char* WINDOW_TITLE = "OpenGL - Water";
constexpr const char* SKYBOX_FACES[6] = {
    "resources/right.jpg",  "resources/left.jpg",  "resources/top.jpg",
    "resources/bottom.jpg", "resources/front.jpg", "resources/back.jpg"};

constexpr float CAMERA_FOV = 60.0f;
constexpr float CAMERA_NEAR = 0.1f;
constexpr float CAMERA_FAR = 2000.0f;

#endif
