#include "Lane.h"

Lane::Lane(float width, Driving_Direction driving_direction)
    : material {}, width  {width}, driving_direction {driving_direction}
{
}


Asphalt_Lane::Asphalt_Lane(float width, Driving_Direction driving_direction)
    : Lane(width, driving_direction)
{
    material.texture_id = model::load_texture("res/textures/asphalt/albedo.png");
    material.normal_map = model::load_texture("res/textures/asphalt/normal.png");
    material.metallic_map = model::load_texture("res/textures/asphalt/metal.png");
    material.roughness_map = model::load_texture("res/textures/asphalt/rough.png");
    material.ao_map = model::load_texture("res/textures/asphalt/ao.png");
}

Mud_Lane::Mud_Lane(float width, Driving_Direction driving_direction)
    : Lane(width, driving_direction)
{
    material.texture_id = model::load_texture("res/textures/ground_mud_rubble/albedo.png");
    material.normal_map = model::load_texture("res/textures/ground_mud_rubble/normal.png");
    material.metallic_map = model::load_texture("res/textures/ground_mud_rubble/metal.png");
    material.roughness_map = model::load_texture("res/textures/ground_mud_rubble/rough.png");
    material.ao_map = model::load_texture("res/textures/ground_mud_rubble/ao.png");
}

Ditch::Ditch(float width)
    : Lane(width, Driving_Direction::none)
{
    material.texture_id = model::load_texture("res/textures/grass/albedo.png");
    material.normal_map = model::load_texture("res/textures/grass/normal.png");
    material.metallic_map = model::load_texture("res/textures/grass/metal.png");
    material.roughness_map = model::load_texture("res/textures/grass/rough.png");
    material.ao_map = model::load_texture("res/textures/grass/ao.png");
}