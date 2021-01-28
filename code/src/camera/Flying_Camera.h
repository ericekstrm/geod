
#include "Camera.h"

class Flying_Camera : public Camera
{
public:
    using Camera::Camera;
    
    void check_input(GLFWwindow* window) override;
};