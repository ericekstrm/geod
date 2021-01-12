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
    velocity = vec3 {0, 0, 0};
    vec3 fly_dir {direction};
    fly_dir.y = 0;
    fly_dir.normalize();
    vec3 bi_dir {up_vector.cross(fly_dir)};

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        velocity += fly_dir * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        velocity -= fly_dir * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        velocity += bi_dir * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        velocity -= bi_dir * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        velocity += up_vector * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        velocity -= up_vector * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        speed *= 1.01;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        speed /= 1.01;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    direction = rotation_matrix((cursor_point_x - xpos)/10, 0, 1, 0) * direction;
    vec3 d = direction.cross(up_vector);
    direction = rotation_matrix((cursor_point_y - ypos)/10, d) * direction;

    glfwSetCursorPos(window, cursor_point_x, cursor_point_y);

}

Matrix4 Camera::get_camera_matrix() const
{
    return look_at(position, position + direction, up_vector);
}
