#pragma once

#include "Vector.h"
#include "Text.h"
#include "Font.h"

class Slider
{
public:

    Slider(vec2 position, float min, float max, float start, Font& font);

    void render() const;

private:

    vec2 position;

    float min;
    float max;
    float current;

    Text text;

};
