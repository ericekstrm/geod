#pragma once

#include "Matrix.h"

// window properties

extern int window_width;
extern int window_height;

// OpenGL projection matrix
extern mat4 projection;

extern int shadow_box_size;
extern mat4 light_projection;

// Terrain
extern int terrain_size;            // Size that the texture "tile" will take up in the world.
extern int terrain_resolution;      // Number of vertices in each dimension of the texture "tile".
extern float terrain_max_height;