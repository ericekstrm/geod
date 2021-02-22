#include "Road.h"

#include <cstdlib>

Road::Road()
    : bezier {}
{
    lanes[1] = std::make_unique<Ditch>(1, bezier);
    lanes[0] = std::make_unique<Mud_Lane>(10, Driving_Direction::left, bezier);
    lanes[-1] = std::make_unique<Ditch>(1, bezier);

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
        data.load_buffer_data(it->second->generate_vertex_data(0));
        data.material = it->second->get_material();
        vao_data.push_back(std::move(data));

        //Adds extra models for the lane, such as the water layer for dirt roads or grass for ditches.
        std::vector<model::Vao_Data> models {it->second->get_lane_models(0)};
        std::move(models.begin(), models.end(), std::back_inserter(vao_data));

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
            data.load_buffer_data(it->second->generate_vertex_data(left_displacement + it->second->get_width() / 2));
            data.material = it->second->get_material();
            vao_data.push_back(std::move(data));

            //Adds extra models for the lane, such as the water layer for dirt roads or grass for ditches.
            std::vector<model::Vao_Data> models {it->second->get_lane_models(left_displacement + it->second->get_width() / 2)};
            std::move(models.begin(), models.end(), std::back_inserter(vao_data));

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
            data.load_buffer_data(it->second->generate_vertex_data(right_displacement - it->second->get_width() / 2));
            data.material = it->second->get_material();
            vao_data.push_back(std::move(data));

            //Adds extra models for the lane, such as the water layer for dirt roads or grass for ditches.
            std::vector<model::Vao_Data> models {it->second->get_lane_models(right_displacement - it->second->get_width() / 2)};
            std::move(models.begin(), models.end(), std::back_inserter(vao_data));

            right_displacement -= it->second->get_width();
            
        } else
        {
            break;
        }
    }
}
