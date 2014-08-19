#pragma once

typedef struct kmMat4 kmMat4;

// Moves the camera by some specified delta value.
void Cam_move(float x, float y);

// Moves the camera to the specified X and Y coordinates.
void Cam_moveTo(float x, float y);

// Zooms the camera by the specified delta value.
void Cam_zoom(float scale_x, float scale_y);

// Zooms the camera the specified scale.
void Cam_zoomTo(float scale_x, float scale_y);

// Returns the camera transform matrix. Ownership is not transferred.
kmMat4* Cam_getTransform();
