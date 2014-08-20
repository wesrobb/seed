#pragma once

typedef struct kmMat4 kmMat4;

// Moves the camera by some specified delta value.
void cam_move(float x, float y);

// Moves the camera to the specified X and Y coordinates.
void cam_move_to(float x, float y);

// Zooms the camera by the specified delta value.
void cam_zoom(float scale_x, float scale_y);

// Zooms the camera the specified scale.
void cam_zoom_to(float scale_x, float scale_y);

// Returns the camera transform matrix. Ownership is not transferred.
kmMat4* cam_transform();
