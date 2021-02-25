#include "Image.h"

#include "model_util.h"

Image::Image(vec2 const& position, vec2 size, std::string image_file)
    : position {position}, size {size}, vao_data {model::get_quad(size)}
{

    vao_data.material.map_albedo = model::load_texture(image_file, false);

    //create_mesh();

}

Image::Image(vec2 const& position, vec2 size, unsigned texture_id)
    : position {position}, size {size}, vao_data {model::get_quad(size)}
{
    vao_data.material.map_albedo = texture_id;
}

void Image::render() const
{
    glDisable(GL_DEPTH_TEST);

    shader.start();
    shader.load_position(position);

    glBindVertexArray(vao_data.vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vao_data.material.map_albedo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDrawElements(GL_TRIANGLES, vao_data.indices_count, GL_UNSIGNED_INT, 0);

    shader.stop();

    glEnable(GL_DEPTH_TEST);
}
