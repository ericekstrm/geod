#include "Lane.h"

Lane::Lane(float width, Driving_Direction driving_direction)
    : material {}, width  {width}, driving_direction {driving_direction}
{
}

model::Buffer_Data Lane::generate_vertex_data_help(Bezier const& bezier, float displacement, Heightmap height_data) const
{
    model::Buffer_Data data {};

    for (int y = 0; y < nr_points; y++)
    {
        vec2 current_point {bezier.get_point(y / nr_points)};
        vec2 road_direction {bezier.get_direction(y / nr_points)};
        vec2 perpendicular_direction {road_direction.y, -road_direction.x};

        for (int x = -(nr_across - 1.0) / 2; x <= (nr_across - 1.0) / 2; x++)
        {
            vec2 point {current_point + perpendicular_direction * (get_width() / (nr_across - 1.0) * x + displacement)};

            data.vertices.push_back(point.x);
            data.vertices.push_back(height_data.get_height(x + (nr_across - 1.0) / 2, y));
            data.vertices.push_back(point.y);

            vec3 normal {height_data.get_normal(x + (nr_across - 1.0) / 2, y)};
            data.normals.push_back(normal.x);
            data.normals.push_back(normal.y);
            data.normals.push_back(normal.z);
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

float Lane::get_quadratic_height(float normalized_point)
{
    return -pow(normalized_point - 0.5, 2) * 4 + 1;
}

// ======================
// ===| Asphalt Lane |===
// ======================

Asphalt_Lane::Asphalt_Lane(float width, Driving_Direction driving_direction)
    : Lane(width, driving_direction)
{
    material.texture_id = model::load_texture("res/textures/asphalt/albedo.png");
    material.normal_map = model::load_texture("res/textures/asphalt/normal.png");
    material.metallic_map = model::load_texture("res/textures/asphalt/metal.png");
    material.roughness_map = model::load_texture("res/textures/asphalt/rough.png");
    material.ao_map = model::load_texture("res/textures/asphalt/ao.png");
}

model::Buffer_Data Asphalt_Lane::generate_vertex_data(Bezier const& bezier, float displacement) const
{
    Heightmap height_data {nr_across, nr_points};
    for (int y = 0; y < nr_points; y++)
    {
        for(int x = 0; x < nr_across; x++) 
        {
            height_data.set(x, y, get_quadratic_height(static_cast<float>(x) / (nr_across - 1)) * max_height);
        }
    }
    return generate_vertex_data_help(bezier, displacement, height_data);
}

// ==================
// ===| Mud Lane |===
// ==================

Mud_Lane::Mud_Lane(float width, Driving_Direction driving_direction)
    : Lane(width, driving_direction)
{
    material.texture_id = model::load_texture("res/textures/ground_mud/albedo.png");
    material.normal_map = model::load_texture("res/textures/ground_mud/normal.png");
    material.metallic_map = model::load_texture("res/textures/ground_mud/metal.png");
    material.roughness_map = model::load_texture("res/textures/ground_mud/rough.png");
    material.ao_map = model::load_texture("res/textures/ground_mud/ao.png");

    /*material.texture_id = model::load_texture("res/textures/ground_mud_rubble/albedo.png");
    material.normal_map = model::load_texture("res/textures/ground_mud_rubble/normal.png");
    material.metallic_map = model::load_texture("res/textures/ground_mud_rubble/metal.png");
    material.roughness_map = model::load_texture("res/textures/ground_mud_rubble/rough.png");
    material.ao_map = model::load_texture("res/textures/ground_mud_rubble/ao.png");*/
}

model::Buffer_Data Mud_Lane::generate_vertex_data(Bezier const& bezier, float displacement) const
{
    Heightmap height_data {nr_across, nr_points};

    //basic surface
    for (int y = 0; y < nr_points; y++)
    {
        for (int x = 0; x < nr_across; x++)
        {
            height_data.set(x, y, 0);
        }
    }

    //tire tracks
    //TODO



    //potholes
    int nr_holes {20};
    for (int i = 0; i < nr_holes; i++)
    {
        int hole_position_across {rand() % (nr_across - 2) + 1};
        int hole_position_along {rand() % (static_cast<int>(nr_points) - 2) + 1};

        height_data.set(hole_position_across, hole_position_along, -2);

    }

    //lowpass filter the result
    height_data.lowpass();

    return generate_vertex_data_help(bezier, displacement, height_data);
}

std::vector<model::Vao_Data> Mud_Lane::get_lane_models(Bezier const& bezier, float displacement) const
{
    std::vector<model::Vao_Data> models {};
    model::Vao_Data vao {};
    Heightmap height_data {nr_across, nr_points};

    //basic surface
    for (int y = 0; y < nr_points; y++)
    {
        for (int x = 0; x < nr_across; x++)
        {
            height_data.set(x, y, -0.2);
        }
    }

    vao.load_buffer_data(generate_vertex_data_help(bezier, displacement, height_data));
    vao.material.texture_id = model::load_texture("res/textures/worn_metal/albedo.png");
    vao.material.normal_map = model::load_texture("res/textures/worn_metal/normal.png");
    vao.material.metallic_map = model::load_texture("res/textures/worn_metal/metal.png");
    vao.material.roughness_map = model::load_texture("res/textures/worn_metal/rough.png");
    vao.material.ao_map = model::load_texture("res/textures/worn_metal/ao.png");

    models.push_back(std::move(vao));
    return models;
}

// ===============
// ===| Ditch |===
// ===============

Ditch::Ditch(float width)
    : Lane(width, Driving_Direction::none)
{
    material.texture_id = model::load_texture("res/textures/grass/albedo.png");
    material.normal_map = model::load_texture("res/textures/grass/normal.png");
    material.metallic_map = model::load_texture("res/textures/grass/metal.png");
    material.roughness_map = model::load_texture("res/textures/grass/rough.png");
    material.ao_map = model::load_texture("res/textures/grass/ao.png");
}

model::Buffer_Data Ditch::generate_vertex_data(Bezier const& bezier, float displacement) const
{
    Heightmap height_data {nr_across, nr_points};

    for (int y = 0; y < nr_points; y++)
    {
        height_data.set(0, y, 0);
        height_data.set(1, y, 0);
        for (int x = 2; x < nr_across; x++)
        {
            height_data.set(x, y, get_quadratic_height(static_cast<float>(x - 2) / (nr_across - 3)) * -get_width() / 2);
        }
    }
    return generate_vertex_data_help(bezier, displacement, height_data);
}
