#pragma once

#include <vector>
#include <utility>
#include <map>

#include "GL/gl.h"

#include "Vector.h"
#include "Font.h"
#include "Shader.h"

class Word
{
public:
    Word(std::string const& word, vec2 const& word_position, Font const& font);

    void render(Shader const& text_shader, Font const& font) const;

    float get_length() const { return word_length; }

private:
    std::map<vec2, Character> characters {};

    float word_length {0};
};
