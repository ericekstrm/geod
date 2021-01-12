
#include "Camera.h"

class Flying_Camera : public Camera
{
public:
    Flying_Camera()
        : Camera{} {}
    Flying_Camera(vec3 const& position)
        : Camera{position} {}
    
    void check_input(GLFWwindow* window) override;
};