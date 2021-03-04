#include "Flying_Camera.h"

#include <cmath>

void Flying_Camera::check_input(GLFWwindow* window)
{
    velocity = vec3 {0, 0, 0};
    vec3 fly_dir {direction};
    fly_dir.y = 0;
    fly_dir.normalize();
    vec3 bi_dir {up_vector.cross(fly_dir)};

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        velocity += fly_dir;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        velocity -= fly_dir;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        velocity += bi_dir;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        velocity -= bi_dir;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        velocity += up_vector;
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        velocity -= up_vector;
    }

    velocity.normalize();
    velocity *= speed;

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

    vec2 window_middle {get_screen_middle()};

    double xoffset = (xpos - window_middle.x) * mouse_sensitivity;
    double yoffset = (window_middle.y - ypos) * mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    direction.x = cos(yaw * 3.14 / 180) * cos(pitch * 3.14 / 180);
    direction.y = sin(pitch * 3.14 / 180);
    direction.z = sin(yaw * 3.14 / 180) * cos(pitch * 3.14 / 180);
    direction.normalize();

    glfwSetCursorPos(window, window_middle.x, window_middle.y);
}
