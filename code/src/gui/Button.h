#pragma once

#include <functional>

#include "Text.h"
#include "Image.h"
#include "Font.h"
#include "Vector.h"

class Button
{
public:
    Button(vec2 const& position, std::string const& text, std::function<void()> const& callback);

    bool check_bounds(vec2 const& position) const;
    void click();
    void try_click(vec2 const& postion);

    void render() const;

private:

    Font font;
    Text text;

    vec2 pos;
    vec2 size;
    
    Image image;

    std::function<void()> callback;

};