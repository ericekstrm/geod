#include "Settings.h"

vec2 Settings::window_size;

void Settings::load()
{
    window_size = vec2{920, 920};
}



// =========================
// ===| OpenGL Settings |===
// =========================

mat4 OpenGL_Settings::projection_matrix;

void OpenGL_Settings::load()
{
    projection_matrix = fov_projection_matrix(90.0f, Settings::get_window_width() / Settings::get_window_height(), 0.001f, 100.0f);
}