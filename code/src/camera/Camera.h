#pragma once

#include <GLFW/glfw3.h>

#include "Vector.h"
#include "Matrix.h"
#include "settings.h"

class Camera
{
public:
    Camera();
    Camera(vec3 const& position);
    Camera(vec3 const& position, vec3 const& direction);
    ~Camera();

    virtual void update(float delta_time);
    virtual void check_input(GLFWwindow* window) = 0;

    Matrix4 get_camera_matrix() const;
    vec3 get_position() const { return position; };

    static vec2 get_screen_middle();

protected:
    vec3 position {5, 2, 5};
    vec3 direction {1, 0, 0};
    vec3 up_vector {0, 1, 0};

    vec3 velocity {};


    float speed = 10;
    float mouse_sensitivity = 0.1f;

    //tmp
    float yaw = 0;
    float pitch = 0;
};
