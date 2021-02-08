#include "Lane.h"

Lane::Lane(float width, Driving_Direction driving_direction)
    : material {}, width  {width}, driving_direction {driving_direction}
{
}


Asphalt_Lane::Asphalt_Lane(float width, Driving_Direction driving_direction)
    : Lane(width, driving_direction)
{
    material.texture_id = model::load_texture("res/textures/pebbled-asphalt1-bl/pebbled_asphalt_albedo.png");
}


Ditch::Ditch(float width)
    : Lane(width, Driving_Direction::none)
{
    material.texture_id = model::load_texture("res/textures/grass1-bl/grass1-albedo3.png");
}
