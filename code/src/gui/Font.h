#pragma once

#include <map>

#include "Vector.h"

class Character;

class Font
{
public:

    /**
     * font_name is the name of the file in "res/font" without the .fnt 
     * font_size is the size in pixels
     */
    Font(std::string const& font_name, int font_size, vec3 const& color = vec3{0,0,0});

    vec3 get_color() const { return color; }
    float get_space_length() const { return space_length; }
    float get_text_height() const { return 0.1; }  //fix this
    Character get_character(int ascii_id) const;
    Character get_character(char ascii_sign) const;
    unsigned get_texture_id() const { return texture_id; }

private:

    vec3 color {};

    std::map<int, Character> characters {};
    float space_length {};

    unsigned texture_id {};

    //help function to extract values from a line in the font file.
    std::map<std::string, int> get__values_from_string(std::string const& line) const;

    void add_character(std::map<std::string, int> & line_values, vec2 const& font_image_size, float scale_factor, float base_line);

};

class Character
{
public:

    //ascii id
    int id;

    //texture space
    float x_texture_coord;
    float y_texture_coord;
    float x_max_texture_coord;
    float y_max_texture_coord;

    //screen space
    float x_offset;
    float y_offset;
    float size_x;
    float size_y;
    float x_advance;

    //opengl 
    unsigned vao {};
    unsigned vb {};
    unsigned tb {};
    unsigned ib {};
    int indices_count {};

    void create_mesh();
};