#pragma once

#include "State.h"
#include "Text.h"
#include "Button.h"
#include "Image.h"

#include "memory"

class Menu_State : public State
{
public:
    Menu_State();
    ~Menu_State();

    virtual void update(float delta_time);
    virtual void render() const;
    virtual void check_input(GLFWwindow* window);

    void activate(GLFWwindow* window) override;
    void deactivate(GLFWwindow* window) override;

    void mouse_button_callback(int button, int action, vec2 const& position) override;

protected:

    void add_button(vec2 const& position, std::string const& text, std::function<void()> const& callback);

private:
    Font font {"arial", 48};
    Text text {"Tree Simulator", vec2{0, -1}, font};

    std::vector<std::unique_ptr<Button>> buttons {};
};
