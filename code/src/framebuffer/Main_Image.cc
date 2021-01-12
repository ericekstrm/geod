#include "Main_Image.h"

Main_Image::Main_Image(unsigned main_pass_id, unsigned god_ray_pass_id)
    : Image {vec2{-1, -1}, vec2{2, 2}, 0}, main_pass_id {main_pass_id}, god_ray_pass_id {god_ray_pass_id}
{

}

void Main_Image::render(vec2 const& sun_pos) const
{
    glDisable(GL_DEPTH_TEST);
    
    post_shader.start();
    post_shader.load_pos_matrix(position);
    post_shader.load_sun_pos(sun_pos);

    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, main_pass_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, god_ray_pass_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);

    post_shader.stop();
    glEnable(GL_DEPTH_TEST);
}