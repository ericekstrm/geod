#pragma once

#include <vector>
#include <utility>
#include <map>

#if (defined (_WIN32) || defined (_WIN64))
    #include "glad/glad.h"
#else
    #include "GL/gl.h"
#endif

#include "Vector.h"
#include "Font.h"
#include "Shader.h"

class Word
{
public:
    Word(std::string const& word, vec2 const& word_position, Font const& font);

    void render(Text_Shader const& shader, Font const& font) const;

    float get_length() const { return word_length; }

private:
    std::map<vec2, Character> characters {};

    float word_length {0};
};
