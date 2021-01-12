#include "Word.h"

#include "Font.h"

Word::Word(std::string const& word, vec2 const& word_position, Font const& font)
{
    vec2 character_position {word_position};
    for (auto it = word.begin(); it != word.end(); it++)
    {
        Character c {font.get_character(*it)};
        characters[character_position] = c;

        character_position.x += c.x_advance;
        word_length += c.x_advance;
    }
}

void Word::render(Text_Shader const& shader, Font const& font) const
{
    for (auto it = characters.begin(); it != characters.end(); it++)
    {
        shader.load_char_pos_matrix(translation_matrix(it->first.x, it->first.y, 0));

        glBindVertexArray(it->second.vao);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(2);


        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, font.get_texture_id());

        glDrawElements(GL_TRIANGLES, it->second.indices_count, GL_UNSIGNED_INT, 0);
    }
}
