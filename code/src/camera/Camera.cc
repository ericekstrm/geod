#include "Camera.h"

#include <iostream>

Camera::Camera()
{
    direction.normalize();
}

Camera::Camera(vec3 const& position)
    : position {position}
{
    direction.normalize();
}

Camera::Camera(vec3 const& position, vec3 const& dir)
    : position {position}, direction {dir}
{
    direction.normalize();
}

Camera::~Camera()
{
}

void Camera::update(float delta_time)
{
    position += velocity * delta_time;
}

void Camera::check_input(GLFWwindow* window)
{
}

Matrix4 Camera::get_camera_matrix() const
{
    return look_at(position, position + direction, up_vector);
}
