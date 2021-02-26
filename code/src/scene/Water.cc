#include "Water.h"

Water_Handler::Water_Handler()
    : reflection_framebuffer {reflection_width, reflection_heigth},
      refraction_framebuffer {refraction_width, refraction_heigth},
      water_tile {}
{

    // set up TEMP tile
    model::Buffer_Data data {};
    data.vertices = {
         0,   height,  0, 
         0,   height, 100, 
        100, height,  0,
        100, height, 100};
    data.normals = {
        0, 1, 0, 
        0, 1, 0, 
        0, 1, 0,
        0, 1, 0};
    data.tangents = {
        0, 0, 1, 
        0, 0, 1, 
        0, 0, 1,
        0, 0, 1,};
    data.texture_coords = {
        0, 0, 
        0, 100, 
        100, 0,
        100, 100,};
    data.indices = { 0, 1, 2, 1, 2, 3 };

    water_tile.load_buffer_data(data);
    water_tile.material.map_albedo = model::load_texture("res/textures/water/albedo.jpg");
    water_tile.material.map_normal = model::load_texture("res/textures/water/normal.jpg");
    water_tile.material.map_metal = model::load_texture("res/textures/water/ao.jpg");
    water_tile.material.map_rough = model::load_texture("res/textures/water/rough.jpg");
    water_tile.material.map_ao = model::load_texture("res/textures/water/ao.jpg");

}

void Water_Handler::bind_reflection_framebuffer() const
{
    reflection_framebuffer.bind();
}

void Water_Handler::bind_refraction_framebuffer() const
{
    refraction_framebuffer.bind();
}

void Water_Handler::unbind_framebuffer() const
{
    reflection_framebuffer.unbind();
}

Framebuffer Water_Handler::get_reflection_framebuffer() const
{
    return reflection_framebuffer;
}

Framebuffer Water_Handler::get_refraction_framebuffer() const
{
    return refraction_framebuffer;
}
