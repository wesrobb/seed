#include <inttypes.h>
#include <stdio.h>

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
        Game_mousePressed();
}

static void mouseMovedCallback(GLFWwindow* window, 
                               double x_pos, double y_pos)
{
        Game_mouseMoved(x_pos, y_pos);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, 
                                            int action, int mods)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
        }
}

static uint32_t _screen_width = 1280;
static uint32_t _screen_height = 720;
static uint32_t _virtual_width = 640;
static uint32_t _virtual_height = 360;

// Game loop based on last implemetation provided at:
// http://www.koonsolo.com/news/dewitters-Gameloop
#define TICKS_PER_SEC 120;
static const double update_interval = 1.0 / TICKS_PER_SEC;
static const double skip_ticks = 1000.0 / TICKS_PER_SEC;
static const uint32_t max_frameskips = 5;

int32_t main(int32_t argc, char* args[])
{
        uint32_t return_code = 1;
        log_init("pong.log", 4);
        LOGDBG("%s", "Game starting");

        glfwSetErrorCallback(errorCallback);

        if (!glfwInit()) {
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

        if (!Game_init(_screen_width, _screen_height,
                       _virtual_width, _virtual_height)) {
                goto cleanup_window;
        }

        double tick = glfwGetTime();
        uint32_t game_updates = 0;
        double interpolation = 0.0f;

        while (!glfwWindowShouldClose(window)) {
                double current_time = glfwGetTime() * 1000; // ms
                while (current_time > tick && game_updates < max_frameskips) {
                       Game_update(update_interval);
                       tick += skip_ticks;
                       game_updates++;
                }

                interpolation = (double)(current_time + skip_ticks - tick) /
                                skip_ticks;
                Game_render(interpolation);
                glfwSwapBuffers(window);
                game_updates = 0;

                glfwPollEvents();
        }
        Game_cleanup();
        LOGDBG("%s", "Game stopping");
        return_code = 0;

cleanup_window:
        glfwDestroyWindow(window);
cleanup_glfw:
        glfwTerminate();

        return return_code;
}
