#include "Slider.h"

Slider::Slider(vec2 position, float min, float max, float start, Font& font)
    : position {position}, min {min}, max {max}, current {start}, text {std::to_string((int)start), position, font}
{
}

void Slider::render() const
{
    text.render();

}