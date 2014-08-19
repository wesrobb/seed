#include "camera.h"

#include <kazmath/kazmath.h>
#include <log/log.h>

static float _x = 0;
static float _y = 0;
static float _scale_x = 1;
static float _scale_y = 1;

void Cam_move(float x, float y)
{
        _x += x;
        _y += y;
}

void Cam_moveTo(float x, float y)
{
        _x = x;
        _y = y;
}

void Cam_zoom(float scale_x, float scale_y)
{
        _scale_x += scale_x;
        _scale_y += scale_y;
}

void Cam_zoomTo(float scale_x, float scale_y)
{
        _scale_x = scale_x;
        _scale_y = scale_y;
}

kmMat4* Cam_getTransform()
{
        static kmMat4 cam;
        kmMat4Identity(&cam);
        kmMat4Translation(&cam, -_x, -_y, 0);
        kmMat4 scale_mat;
        kmMat4Scaling(&scale_mat, _scale_x, _scale_y, 1);
        kmMat4Multiply(&cam, &cam, &scale_mat);
        return &cam;
}
