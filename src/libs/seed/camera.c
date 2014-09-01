#include "camera.h"

#include <kazmath/kazmath.h>

#include "log.h"

static float _x = 0;
static float _y = 0;
static float _scale_x = 1;
static float _scale_y = 1;

void cam_move(float x, float y)
{
        _x += x;
        _y += y;
}

void cam_move_to(float x, float y)
{
        _x = x;
        _y = y;
}

void cam_zoom(float scale_x, float scale_y)
{
        _scale_x += scale_x;
        _scale_y += scale_y;
}

void cam_zoom_to(float scale_x, float scale_y)
{
        _scale_x = scale_x;
        _scale_y = scale_y;
}

kmMat4* cam_transform()
{
        static kmMat4 cam;
        kmMat4Identity(&cam);
        kmMat4Translation(&cam, -_x, -_y, 0);
        kmMat4 scale_mat;
        kmMat4Scaling(&scale_mat, _scale_x, _scale_y, 1);
        kmMat4Multiply(&cam, &cam, &scale_mat);
        return &cam;
}
