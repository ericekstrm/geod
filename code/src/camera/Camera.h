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
    virtual void check_input(GLFWwindow* window);

    Matrix4 get_camera_matrix() const;
    vec3 get_position() const { return position; };

protected:
    vec3 position {5, 2, 5};
    vec3 direction {-5, -5, -5};
    vec3 up_vector {0, 1, 0};

    vec3 velocity {};
    float speed = 20;

    double cursor_point_x { static_cast<double>(window_width / 2.0)};
    double cursor_point_y {static_cast<double>(window_height / 2.0)};
};
