#include "Font.h"

#include "GL/gl.h"

#include "model_util.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Vector.h"

/*
    This constructor decomposes a .fnt file and extracts the useful parts. 
*/
Font::Font(std::string const& font_name, int font_size, vec3 const& color)
    : color {color}
{
    std::ifstream font_file {"res/fonts/" + font_name + ".fnt"};

    if (!font_file.is_open())
    {
        std::cout << "Could not open font: "  << font_name << std::endl;
    }

    std::string curline;
    std::getline(font_file, curline);
    std::map<std::string, int> line_values {get__values_from_string(curline)};

    int default_font_size {line_values["size"]};

    std::getline(font_file, curline);
    line_values = get__values_from_string(curline);

    vec2 font_image_size {static_cast<float>(line_values["scaleW"]), static_cast<float>(line_values["scaleH"])};
    float base_line {line_values["base"] / font_image_size.y * font_size / default_font_size};

    std::getline(font_file, curline);
    std::getline(font_file, curline);

	while (std::getline(font_file, curline))
	{
        if (curline.rfind("char", 0) != 0)
        {
            //line does not start with 'char'
            continue;
        }

        line_values = get__values_from_string(curline);

        add_character(line_values, font_image_size, static_cast<float>(font_size) / default_font_size, base_line);
    }

    texture_id = model::load_texture("res/fonts/" + font_name + ".png", false);
}

std::map<std::string, int> Font::get__values_from_string(std::string const& line) const
{
    std::map<std::string, int> line_values {};

    std::istringstream iss {line};

    while (true)
    {
        std::string part;
        iss >> part;

        if (!iss)
        {
            break;
        }

        std::size_t found = part.find("=");
        if (found != std::string::npos)
        {
            std::string name {part.substr(0, found)};
            std::string value {part.substr(found + 1, part.length())};

            try 
            {
                line_values[name] = std::stoi(value);
            } catch(std::exception const&)
            {
                line_values[name] = 0;
            }

        }
    }

    return line_values;

}

void Font::add_character(std::map<std::string, int> & line_values, vec2 const& font_image_size, float scale_factor, float base_line)
{
    //the ascii id of character
    int id = line_values["id"];

    //texture coordinates of the character. normalized to [1,-1]
    float x_tex = line_values["x"] / font_image_size.x;
    float y_tex = line_values["y"] / font_image_size.y;

    //the width of the box that should be extracted from the texture. also normalized.
    float tex_width = line_values["width"] / font_image_size.x;
    float tex_height = line_values["height"] / font_image_size.y;

    //the size of the quad in screen space
    float quad_width = tex_width * scale_factor;
    float quad_height = tex_height * scale_factor;

    //how much to offset the quad in screen space.
    float x_offset = line_values["xoffset"] / font_image_size.x * scale_factor;
    float y_offset = base_line - quad_height - line_values["yoffset"] / font_image_size.y * scale_factor;

    //how much to advance the cursor to the next character
    float x_advance = line_values["xadvance"] / font_image_size.x * scale_factor;

    Character c {id, x_tex, y_tex, tex_width, tex_height, x_offset, y_offset, quad_width, quad_height, x_advance};
    c.create_mesh();

    if (c.id == 32)
    {
        space_length = c.x_advance;
    } else
    {
        characters[c.id] = c;
    }
}

Character Font::get_character(int ascii_id) const
{
    return characters.at(ascii_id);
}

Character Font::get_character(char ascii_sign) const
{
    return characters.at((int) ascii_sign);
}

// ===================
// ===| Character |===
// ===================

void Character::create_mesh()
{
    std::vector<float> vertices {
        x_offset, y_offset, 0,
        x_offset + size_x, y_offset, 0,
        x_offset, y_offset + size_y, 0,
        x_offset + size_x, y_offset + size_y, 0
    };
    std::vector<float> texture_coords {
        x_texture_coord, y_texture_coord + y_max_texture_coord,
        x_texture_coord + x_max_texture_coord, y_texture_coord + y_max_texture_coord,
        x_texture_coord, y_texture_coord,
        x_texture_coord + x_max_texture_coord, y_texture_coord
    };

    std::vector<int> indices { 0, 1, 2, 1, 2, 3 };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    int vertices_attrib_array = 0;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(vertices_attrib_array, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    int texture_attrib_array = 2;
    glGenBuffers(1, &tb);
    glBindBuffer(GL_ARRAY_BUFFER, tb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texture_coords.size(), &texture_coords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(texture_attrib_array, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);
    indices_count = indices.size();

    glEnableVertexAttribArray(vertices_attrib_array);
    glEnableVertexAttribArray(texture_attrib_array);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}