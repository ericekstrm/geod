#include "Road.h"

Road::Road()
{
    generate_road();
    model_data.load_buffer_data(data);
    model_data.material.texture_id = model::load_texture("res/terrain/textures/cobblestone.jpeg");
}

void Road::generate_road()
{
    //Creating a staight road

    vec2 start_point {0, 0};
    vec2 stop_point {20, 20};
    float nr_points {10};
    float road_width = 5;

    for (int i = 0; i <= nr_points; i++)
    {
        vec2 current_point {start_point + (stop_point - start_point) * i / nr_points};
        vec2 road_direction = stop_point - start_point;
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