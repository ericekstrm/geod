#include "Button.h"

Button::Button(vec2 const& position, std::string const& text_string, std::function<void()> const& callback)
    : font {"arial", 48},
      text {text_string, position + vec2{0.015, 0.015}, font},
      pos {position}, 
      size {vec2{text.get_length(), font.get_text_height()}},
      image {position, size, "res/images/button.png"}, 
      callback {callback}
{
}

bool Button::check_bounds(vec2 const& p) const
{
    return (p.x >= pos.x && p.x <= pos.x + size.x && p.y >= pos.y && p.y <= pos.y + size.y);
}

void Button::click()
{
    callback();
}

void Button::try_click(vec2 const& p)
{
    if (check_bounds(p))
    {
        click();
    }
}

void Button::render() const
{
    image.render();
    text.render();
}