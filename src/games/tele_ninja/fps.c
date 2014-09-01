#include "fps.h"

#include <inttypes.h>
#include <string.h>

#include <glfw/glfw3.h>

#include <seed/log.h>

#define NUM_FRAMES 120
static double frame_times[NUM_FRAMES];
static double frame_time_last;
static uint32_t frame_count;
static double log_interval; // Time between draw calls in sec.
static double last_log_time;

double Fps_think()
{
        // save the frame time value between calls
        double ticks = glfwGetTime();
        frame_times[frame_count % NUM_FRAMES] = ticks - frame_time_last;
        frame_time_last = ticks;
        frame_count++;
        return ticks;
}

double Fps_calc()
{
        // Work out the current framerate
        uint32_t count;
        if (frame_count < NUM_FRAMES) {
                count = frame_count;
        } else {
                count = NUM_FRAMES;
        }

        // Add up all the values and divide to get the average frame time.
        double new_fps = 0;
        for (uint32_t i = 0; i < count; i++) {
                new_fps += frame_times[i];
        }
        new_fps /= count;

        // Now to make it an actual frames per second value...
        return 1 / new_fps;
}

void Fps_init()
{
        memset(frame_times, 0, sizeof(frame_times));
        frame_count = 0;
        frame_time_last = glfwGetTime();
        log_interval = 5.0;
        last_log_time = 0.0;
}

void Fps_log()
{
        double ticks = Fps_think();
        if (ticks <= last_log_time + log_interval) {
                return;
        }

        last_log_time = ticks;
        LOGDBG("FPS: %.0f", Fps_calc());
}
