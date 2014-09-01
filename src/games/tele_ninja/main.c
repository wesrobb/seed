#include <inttypes.h>
#include <stdio.h>

#include <Windows.h>

#include <core/thread.h>
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <log/log.h>

#include "game.h"

static void errorCallback(int error, const char* description)
{
        LOGERR("Error %d: %s", error, description);
}

static void mouseButtonCallback(GLFWwindow* window, 
                                int button, int action, int mods)
{
        game_mouse_pressed();
}

static void mouseMovedCallback(GLFWwindow* window, 
                               double x_pos, double y_pos)
{
        game_mouse_moved(x_pos, y_pos);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, 
                                            int action, int mods)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (action == GLFW_PRESS) {
                game_key_pressed(key);
        }

        if (action == GLFW_RELEASE) {
                game_key_released(key);
        }
}

static uint32_t _screen_width = 1280;
static uint32_t _screen_height = 720;
static uint32_t _virtual_width = 576;
static uint32_t _virtual_height = 320;

#define EXPECTED_FRAME_TIME 8.3 // ms per frame

int32_t main(int32_t argc, char* args[])
{
        uint32_t return_code = 1;
        log_init("pong.log", 4);
        LOGDBG("%s", "Game starting");

        glfwSetErrorCallback(errorCallback);

        if (!glfwInit()) {
                LOGERR("%s", "Failed to initialize GLFW");
                return return_code;
        }

        //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        GLFWwindow* window = glfwCreateWindow(_screen_width, _screen_height,
                                              "PongHD", NULL, NULL);
        if (!window) {
                goto cleanup_glfw;
        }

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouseMovedCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwMakeContextCurrent(window);

        GLenum err = glewInit();
        if (err != GLEW_OK) {
                LOGERR("GLEW unable to be initialized: %s",
                       glewGetErrorString(err));
                return return_code;
        }

        if (!game_init(window, _virtual_width, _virtual_height)) {
                goto cleanup_window;
        }

        double tick = glfwGetTime();
        uint32_t game_updates = 0;
        double interpolation = 0.0f;

        while (!glfwWindowShouldClose(window)) {
                double start_time = glfwGetTime() * 1000; // ms
                game_update(EXPECTED_FRAME_TIME);
                glfwPollEvents();

                double frame_time = (glfwGetTime() * 1000) - start_time;
                if (frame_time < EXPECTED_FRAME_TIME) {
                        Sleep((DWORD)(EXPECTED_FRAME_TIME - frame_time));
                }

                double total_time = (glfwGetTime() * 1000) - start_time;
                static uint32_t frame_count = 0;
                if (++frame_count % 500 == 0) {
                        LOGINFO("Frame_time %fms", total_time);
                }
        }
        game_cleanup();
        LOGDBG("%s", "Game stopping");
        return_code = 0;

cleanup_window:
        glfwDestroyWindow(window);
cleanup_glfw:
        glfwTerminate();

        return return_code;
}
