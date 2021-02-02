#pragma once

#include "Vector.h"
#include "Matrix.h"

class Settings
{
public:

    static void load();
    static void save();
    static void set_defaults();

    static vec2 get_window_size() { return window_size; }
    static float get_window_width() { return window_size.x; }
    static float get_window_height() { return window_size.y; }

private:
    Settings() = delete;

    static vec2 str_to_vec2(std::string const& str);

    //window properties
    static vec2 window_size;
    //TODO: fullscreen?

};

class OpenGL_Settings
{
public:

    static void load();

    static mat4 get_projection_matrix() { return projection_matrix; }

private:
    OpenGL_Settings() = delete;

    static mat4 projection_matrix;

};
