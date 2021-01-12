#include "Image.h"

#include "model_util.h"

Image::Image(vec2 const& position, vec2 size, std::string image_file)
    : position {position}, size {size}
{
    texture_id = model::load_texture(image_file, false);

    create_mesh();

}

Image::Image(vec2 const& position, vec2 size, unsigned texture_id)
    : position {position}, size {size}, texture_id {texture_id}
{
    create_mesh();
}

void Image::render() const
{
    glDisable(GL_DEPTH_TEST);

    shader.start();
    shader.load_pos_matrix(position);

    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);

    shader.stop();

    glEnable(GL_DEPTH_TEST);
}

void Image::create_mesh()
{
    std::vector<float> vertices {
        0,      0,      0,
        size.x, 0,      0,
        0,      size.y, 0,
        size.x, size.y, 0
    };
    std::vector<float> texture_coords {
        0, 0,
        1, 0,
        0, 1,
        1, 1
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