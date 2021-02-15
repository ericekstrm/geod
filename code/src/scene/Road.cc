#include "Road.h"

#include <cstdlib>

Road::Road()
    : bezier {}
{
    lanes[1] = std::make_unique<Ditch>(1);
    lanes[0] = std::make_unique<Mud_Lane>(5, Driving_Direction::left);
    lanes[-1] = std::make_unique<Ditch>(1);

    generate_all_vertex_data();
}

float Road::get_total_width() const
{
    float width {};
    for (auto const& lane : lanes)
    {
        width += lane.second->get_width();
    }
    return width;
}

float Road::get_lane_width(int index) const
{
    auto it = lanes.find(index);
    if (it != lanes.end())
    {
        return it->second->get_width();
    }
    return 0;
}


//===================================================================================================================================

void Road::generate_all_vertex_data()
{

    float center_lane_width {0};
    auto it = lanes.find(0);
    if (it != lanes.end())
    {
        model::Vao_Data data {};
        data.load_buffer_data(generate_vertex_data(bezier, *it->second, 0));
        data.material = it->second->get_material();
        vao_data.push_back(std::move(data));

        center_lane_width = it->second->get_width();
    }

    float left_displacement {center_lane_width / 2};
    float right_displacement {-center_lane_width / 2};
    

    for (int i = 1; true; i++)
    {
        auto it = lanes.find(i);
        if (it != lanes.end())
        {
            model::Vao_Data data {};
            data.load_buffer_data(generate_vertex_data(bezier, *it->second, left_displacement + it->second->get_width() / 2));
            data.material = it->second->get_material();
            vao_data.push_back(std::move(data));

            left_displacement += it->second->get_width();
            
        } else
        {
            break;
        }
    }

    for (int i = -1; true; i--)
    {
        auto it = lanes.find(i);
        if (it != lanes.end())
        {
            model::Vao_Data data {};
            data.load_buffer_data(generate_vertex_data(bezier, *it->second, right_displacement - it->second->get_width() / 2));
            data.material = it->second->get_material();
            vao_data.push_back(std::move(data));

            right_displacement -= it->second->get_width();
            
        } else
        {
            break;
        }
    }
}

model::Buffer_Data Road::generate_vertex_data(Bezier const& bezier, Lane const& lane, float displacement)
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
            vec2 point {current_point + perpendicular_direction * (lane.get_width() / (nr_across - 1.0) * x + displacement)};

            data.vertices.push_back(point.x);
            data.vertices.push_back(1);
            data.vertices.push_back(point.y);

            data.normals.push_back(0);
            data.normals.push_back(1);
            data.normals.push_back(0);
            data.tangents.push_back(1);
            data.tangents.push_back(0);
            data.tangents.push_back(0);

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
