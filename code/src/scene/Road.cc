#include "Road.h"

#include <cstdlib>

Road::Road(Road_Bezier const& bezier)
{
    model_data.load_buffer_data(generate_vertex_data(bezier));
    model_data.material.texture_id = model::load_texture("res/textures/pebbled-asphalt1-bl/pebbled_asphalt_albedo.png");
}

model::Buffer_Data Road::generate_vertex_data(Road_Bezier const& bezier)
{
    model::Buffer_Data data {};
    
    //TODO: tmp variables
    float nr_points {100};
    int nr_across {11}; // has to be odd!

    for (int i = 0; i <= nr_points; i++)
    {
        vec2 current_point {bezier.get_point(i / nr_points)};
        vec2 road_direction {bezier.get_direction(i / nr_points)};
        vec2 perpendicular_direction {road_direction.y, -road_direction.x};

        for (int x = -(nr_across - 1.0) / 2; x <= (nr_across - 1.0) / 2; x++)
        {
            vec2 point {current_point + perpendicular_direction * bezier.get_width() / (nr_across - 1.0) * x };

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

    return data;
}
