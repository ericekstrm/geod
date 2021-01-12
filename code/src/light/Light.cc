#include "Light.h"

void Light_Container::add_pos_light(vec3 const& position,  vec3 const& color)
{
    pos_lights.push_back(Pos_Light {position, color});
}

void Light_Container::add_dir_light(vec3 const& direction, vec3 const& color)
{
    dir_lights.push_back(Dir_Light {direction, color});
}

void Light_Container::render(mat4 const& camera_matrix) const
{
    light_shader.start();
    light_shader.load_projection_matrix();
    light_shader.load_camera_matrix(camera_matrix);
    for (auto it = pos_lights.begin(); it != pos_lights.end(); it++)
    {
        glBindVertexArray(pos_light_vao_data.vao);

        light_shader.load_position(it->get_position());
        light_shader.load_color(it->get_color());

        glDrawElements(GL_TRIANGLES, pos_light_vao_data.indices_count, GL_UNSIGNED_INT, 0);
    }

    light_shader.stop();
}

void Light_Container::render_sun(Camera const * camera) const
{
    sun.render(camera);
}

void Light_Container::update(float delta_time)
{
    sun.update(delta_time);
}

vec2 Light_Container::get_sun_screen_position(Camera const * camera) const
{
    vec3 tmp {projection * camera->get_camera_matrix().remove_translation() * sun.get_position()};
    return {(tmp.x + 1) / 2, (tmp.y + 1) / 2};
}

std::vector<vec3> Light_Container::get_pos_dir_data() const
{
    std::vector<vec3> pos_dir_data {};
    for (auto it = pos_lights.begin(); it != pos_lights.end(); it++)
    {
        pos_dir_data.push_back(it->get_position());
    }
    for (auto it = dir_lights.begin(); it != dir_lights.end(); it++)
    {
        pos_dir_data.push_back(it->get_direction());
    }
    pos_dir_data.push_back(sun.get_direction());
    return pos_dir_data;
}

std::vector<vec3> Light_Container::get_color_data() const 
{
    std::vector<vec3> color_data {};
    for (auto it = pos_lights.begin(); it != pos_lights.end(); it++)
    {
        color_data.push_back(it->get_color());
    }
    for (auto it = dir_lights.begin(); it != dir_lights.end(); it++)
    {
        color_data.push_back(it->get_color());
    }
    color_data.push_back(sun.get_color());
    return color_data;
}

std::vector<vec3> Light_Container::get_attenuation_data() const
{
    std::vector<vec3> attenuation_data {};
    for (auto it = pos_lights.begin(); it != pos_lights.end(); it++)
    {
        attenuation_data.push_back(vec3{it->get_attenuation_constant(), it->get_attenuation_linear(), it->get_attenuation_quadratic()});
    }
    for (auto it = dir_lights.begin(); it != dir_lights.end(); it++)
    {
        attenuation_data.push_back(vec3{0, 0, 0});
    }
    attenuation_data.push_back(vec3{0, 0, 0});
    return attenuation_data; 
}

std::vector<int> Light_Container::get_light_type_data() const
{
    std::vector<int> light_type_data {};
    for (auto it = pos_lights.begin(); it != pos_lights.end(); it++)
    {
        light_type_data.push_back(0);
    }
    for (auto it = dir_lights.begin(); it != dir_lights.end(); it++)
    {
        light_type_data.push_back(1);
    }
    light_type_data.push_back(1);
    return light_type_data; 
}

// =============
// ===| Sun |===
// =============

Sun::Sun()
    : Dir_Light(vec3{0,0,0}, vec3{0.494, 0.4155, 0.1255})
{
}

Sun::~Sun()
{

}

void Sun::render(Camera const * camera) const
{
    glDisable(GL_DEPTH_TEST);

    shader.start();
    shader.load_projection_matrix();
    shader.load_camera_matrix(camera->get_camera_matrix().remove_translation());
    shader.load_model_matrix(translation_matrix(pos) * rot * scale_matrix(5,5,5));
    vec3 color {255 / 255.0, 147 / 255.0, 41 / 255.0};
    shader.load_color(color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, billboard.material.texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindVertexArray(billboard.vao);
    glDrawElements(GL_TRIANGLES, billboard.indices_count, GL_UNSIGNED_INT, 0);

    shader.stop();
    glEnable(GL_DEPTH_TEST);
}

void Sun::update(float delta_time)
{
    pos = rotation_matrix(delta_time / 2, vec3{0,0,1}) * pos;

    dir = vec3{-1,-1,0}; //(pos * -1).normalize();

    // Rotate the sun so that it always faces its rotation center
    rot = rotation_matrix(vec3{0,0,1}, dir);
}