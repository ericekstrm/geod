#include "Text.h"

#include <sstream>

Text::Text(std::string const& text_string, Font const& font)
    : Text {text_string, vec2 {0,0}, font}
{
}

/*
 * Position of the bottom left point, given in normalized coordinates [-1, 1]
 *
*/
Text::Text(std::string const& text_string, vec2 const& position, Font const& font)
    : font {font}, position {position}
{
    set_text(text_string);
    /*std::stringstream ss {text_string};

    std::string word {};
    vec2 word_postion {0, 0};
    while (ss >> word)
    {
        Word w {word, word_postion, font};
        words.push_back(w);

        word_postion.x += font.get_space_length();
        word_postion.x += w.get_length();

        text_length += font.get_space_length();
        text_length += w.get_length();
    }*/
}

void Text::render() const
{
    glDisable(GL_DEPTH_TEST);
    shader.start();

    shader.load_font_color(font.get_color());
    shader.load_text_pos_matrix(translation_matrix(position.x, position.y, 0));

    for (auto it = words.begin(); it != words.end(); it++)
    {
        it->render(shader, font);
    }

    shader.stop();
    glEnable(GL_DEPTH_TEST);
}

void Text::set_text(std::string const& new_text)
{
    //reset text
    words.clear();
    text_length = 0;

    std::stringstream ss {new_text};

    std::string word {};
    vec2 word_postion {0, 0};
    while (ss >> word)
    {
        Word w {word, word_postion, font};
        words.push_back(w);

        word_postion.x += font.get_space_length();
        word_postion.x += w.get_length();

        text_length += font.get_space_length();
        text_length += w.get_length();
    }
}