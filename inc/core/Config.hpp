#ifndef CONFIG_HPP
#define CONFIG_HPP

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr const char* WINDOW_TITLE = "OpenGL - Water";

constexpr const char* SKYBOX_FACES[6] = {
    "resources/right.png",  "resources/left.png",  "resources/top.png",
    "resources/bottom.png", "resources/front.png", "resources/back.png"};

constexpr int MAX_WAVE_ITERATION = 64;

constexpr float CAMERA_FOV = 60.0f;
constexpr float CAMERA_NEAR = 0.1f;
constexpr float CAMERA_FAR = 2000.0f;

#endif
