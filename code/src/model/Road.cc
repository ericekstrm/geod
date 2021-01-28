#include "Road.h"

Road::Road()
{
    generate_control_points();
    generate_vertex_data();
    model_data.load_buffer_data(data);
    model_data.material.texture_id = model::load_texture("res/terrain/textures/cobblestone.jpeg");
}

void Road::generate_control_points()
{
    //set start and stop points
    control_points[0] = vec2{0, 0};
    control_points[3] = vec2{40, 40};

    //set middle points
    control_points[1] = vec2{40, 0};
    control_points[2] = vec2{40, 0};
}

void Road::generate_vertex_data()
{
    //Creating a staight road
    float nr_points {20};
    float road_width = 5;

    for (int i = 0; i <= nr_points; i++)
    {
        vec2 current_point {bezier_point(i / nr_points)};
        vec2 road_direction {bezier_direction(i / nr_points)};
        road_direction.normalize();

        vec2 perpendicular_direction {road_direction.y, -road_direction.x};

        vec2 left_point  {current_point - perpendicular_direction * road_width / 2};
        vec2 right_point {current_point + perpendicular_direction * road_width / 2};

        data.vertices.push_back(left_point.x);
        data.vertices.push_back(1);
        data.vertices.push_back(left_point.y);

        data.normals.push_back(0);
        data.normals.push_back(1);
        data.normals.push_back(0);

        data.texture_coords.push_back(1);
        data.texture_coords.push_back(1);

        data.vertices.push_back(right_point.x);
        data.vertices.push_back(1);
        data.vertices.push_back(right_point.y);

        data.normals.push_back(0);
        data.normals.push_back(1);
        data.normals.push_back(0);

        data.texture_coords.push_back(1);
        data.texture_coords.push_back(1);

    }

    for(int i = 0; i < nr_points; i++)
    {
        data.indices.push_back(i * 2);
        data.indices.push_back(i * 2 + 1);
        data.indices.push_back(i * 2 + 2);

        data.indices.push_back(i * 2 + 1);
        data.indices.push_back(i * 2 + 3);
        data.indices.push_back(i * 2 + 2);
    }
}

vec2 Road::bezier_point(float u)
{
    return control_points[0] * pow(1 - u, 3) 
         + control_points[1] * 3 * u * pow(1 - u, 2)
         + control_points[2] * 3 * (1 - u) * pow(u, 2)
         + control_points[3] * pow(u, 3);
}

vec2 Road::bezier_direction(float u)
{
    return control_points[0] * -3 * pow(1 - u, 2) 
         + control_points[1] * (3 * pow(1 - u, 2) - 6 * u * (1 - u))
         + control_points[2] * (-3 * pow(u, 2) + 6 * (1 - u) * u)
         + control_points[3] * 3 * pow(u, 2);
}