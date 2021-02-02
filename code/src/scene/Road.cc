#include "Road.h"

#include <cstdlib>

Road::Road()
{
    generate_bezier();
    generate_vertex_data();
    model_data.load_buffer_data(data);
    model_data.material.texture_id = model::load_texture("res/textures/pebbled-asphalt1-bl/pebbled_asphalt_albedo.png");
}

bool Road::is_on(vec2 const& p) const
{
    for (int i = 0; i < 100; i++)
    {
        float u = i / 100.0;
        vec2 point = bezier_point(u);
        float l = (p - point).length();
        if (l < width / 2)
        {
            return true;
        }
    }

    return false;
}

float Road::distance_to(vec2 const& p) const
{
    float min_dist = (p - control_points[0]).length();
    for (int i = 0; i < 100; i++)
    {
        float u = i / 100.0;
        vec2 point = bezier_point(u);
        float l = (p - point).length();
        if (l < min_dist)
        {
            min_dist = l;
        }
    }

    return min_dist;
}


void Road::generate_bezier()
{
    //set start and stop points
    control_points[0] = vec2{0, 0};
    control_points[3] = vec2{40, 40};

    //set middle points
    control_points[1] = vec2{40, 0};
    control_points[2] = vec2{40, 0};

    //set length of road

}

void Road::generate_vertex_data()
{
    //Creating a staight road
    float nr_points {100};
    float nr_across {5}; // has to be odd!

    for (int i = 0; i <= nr_points; i++)
    {
        vec2 current_point {bezier_point(i / nr_points)};
        vec2 road_direction {bezier_direction(i / nr_points)};
        road_direction.normalize();

        vec2 perpendicular_direction {road_direction.y, -road_direction.x};

        for (int x = -(nr_across - 1) / 2; x <= (nr_across - 1) / 2; x++)
        {
            vec2 point {current_point + perpendicular_direction * (width / 2.0 / (nr_across - 1) / 2 * x) };

            data.vertices.push_back(point.x);
            data.vertices.push_back(1);
            data.vertices.push_back(point.y);

            data.normals.push_back(0);
            data.normals.push_back(1);
            data.normals.push_back(0);

            data.texture_coords.push_back(point.x / 2);
            data.texture_coords.push_back(point.y / 2);

        }

    }

    for (int y = 0; y < nr_points - 1; y++)
    {
        for (int x = 0; x < nr_across - 1; x++)
        {
            int bottom_left = y * nr_across + x;
            int top_left = bottom_left + nr_across;
            int bottom_right = bottom_left + 1;
            int top_right = bottom_right + nr_across;

            data.indices.push_back(top_right);
            data.indices.push_back(bottom_left);
            data.indices.push_back(top_left);
            data.indices.push_back(bottom_right);
            data.indices.push_back(bottom_left);
            data.indices.push_back(top_right);
        }
    }
}

vec2 Road::bezier_point(float u) const
{
    return control_points[0] * pow(1 - u, 3) 
         + control_points[1] * 3 * u * pow(1 - u, 2)
         + control_points[2] * 3 * (1 - u) * pow(u, 2)
         + control_points[3] * pow(u, 3);
}

vec2 Road::bezier_direction(float u) const
{
    return control_points[0] * -3 * pow(1 - u, 2) 
         + control_points[1] * (3 * pow(1 - u, 2) - 6 * u * (1 - u))
         + control_points[2] * (-3 * pow(u, 2) + 6 * (1 - u) * u)
         + control_points[3] * 3 * pow(u, 2);
}