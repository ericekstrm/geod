#pragma once
#include "Camera.h"
class Third_Person_Camera : public Camera
{
public:
    using Camera::Camera;

    void check_input(GLFWwindow* window) override;
};

