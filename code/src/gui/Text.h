#pragma once

#include <string>
#include <vector>

#include "Vector.h"
#include "Font.h"
#include "Word.h"
#include "Shader.h"

class Text
{
public:

    Text(std::string const& text_string, Font const& font);
    Text(std::string const& text_string, vec2 const& position, Font const& font);

    float get_length() const { return text_length; }

    void render() const;

private:

    Font const& font;

    std::string text_string;

    vec2 position;
    vec3 color;

    float text_length {0};

    std::vector<Word> words {};

    Text_Shader shader {};

};
