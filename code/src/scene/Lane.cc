#include "Lane.h"

Lane::Lane(float width, Driving_Direction driving_direction, Bezier& bezier)
    : width  {width}, driving_direction {driving_direction}, bezier {bezier}, vao_data {}
{
    //set the nr of points in the mesh based on the width and length of the lane
    nr_points = floor(bezier.get_length() / mesh_grid_size);
    nr_across = width / mesh_grid_size;
    if (nr_across % 2 == 0)
    {
        nr_across++;
    }
}

model::Buffer_Data Lane::generate_vertex_data_help(float displacement, Lane_Heightmap height_data) const
{
    model::Buffer_Data data {};

    for (int index_z = 0; index_z < nr_points; index_z++)
    {
        for (int index_x = 0; index_x < nr_across; index_x++)
        {
            vec2 point {height_data.get_position(index_x, index_z)};
            float height {height_data.get_height(index_x, index_z)};

            data.vertices.push_back(point.x);
            data.vertices.push_back(height);
            data.vertices.push_back(point.y);

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

    //calculate normals
    std::vector<vec3> normals {};
    std::vector<int> normal_count {};
    normals.resize(data.vertices.size() / 3);
    normal_count.resize(data.vertices.size() / 3);
    for (int i = 0; i < data.indices.size(); i += 3)
    {
        vec3 p1 {data.vertices[data.indices[i] * 3], data.vertices[data.indices[i] * 3 + 1], data.vertices[data.indices[i] * 3 + 2]};
        vec3 p2 {data.vertices[data.indices[i + 1] * 3], data.vertices[data.indices[i + 1] * 3 + 1], data.vertices[data.indices[i + 1] * 3 + 2]};
        vec3 p3 {data.vertices[data.indices[i + 2] * 3], data.vertices[data.indices[i + 2] * 3 + 1], data.vertices[data.indices[i + 2] * 3 + 2]};

        vec3 w = p3 - p1;
        vec3 v = p2 - p1;

        vec3 n = v.cross(w).normalize();

        normals[data.indices[i]] += n;
        normals[data.indices[i + 1]] += n;
        normals[data.indices[i + 2]] += n;
        normal_count[data.indices[i]]++;
        normal_count[data.indices[i + 1]]++;
        normal_count[data.indices[i + 2]]++;
    }

    for (int i = 0; i < normals.size(); i++)
    {
        vec3 normal {normals[i] / normal_count[i]};

        data.normals.push_back(normal.x);
        data.normals.push_back(normal.y);
        data.normals.push_back(normal.z);

        vec3 tangent {normal.cross(vec3{0,0,1})};
        if (normal == vec3{0,0,1})
        {
            tangent = vec3{1,0,0};
        }
        if (tangent.x < 0)
        {
            tangent *= -1;
        }
        tangent.normalize();

        data.tangents.push_back(tangent.x);
        data.tangents.push_back(tangent.y);
        data.tangents.push_back(tangent.z);
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

Asphalt_Lane::Asphalt_Lane(float width, Driving_Direction driving_direction, Bezier& bezier)
    : Lane(width, driving_direction, bezier)
{
    vao_data.material.map_albedo = model::load_texture("res/textures/asphalt/albedo.png");
    vao_data.material.map_normal = model::load_texture("res/textures/asphalt/normal.png");
    vao_data.material.map_metal = model::load_texture("res/textures/no_metal.png");
    vao_data.material.map_rough = model::load_texture("res/textures/asphalt/rough.png");
    vao_data.material.map_ao = model::load_texture("res/textures/asphalt/ao.png");
}

void Asphalt_Lane::generate_vertex_data(float displacement)
{
    Lane_Heightmap height_data {nr_across, nr_points, bezier, displacement, get_width()};
    for (int y = 0; y < nr_points; y++)
    {
        for(int x = 0; x < nr_across; x++) 
        {
            height_data.update_height(x, y, get_quadratic_height(static_cast<float>(x) / (nr_across - 1)) * max_height);
        }
    }
    vao_data.load_buffer_data(generate_vertex_data_help(displacement, height_data));
}

// ==================
// ===| Mud Lane |===
// ==================

Mud_Lane::Mud_Lane(float width, Driving_Direction driving_direction, Bezier& bezier)
    : Lane(width, driving_direction, bezier), water_surface {}
{
    vao_data.material.map_albedo = model::load_texture("res/textures/gravel_01/albedo.png");
    vao_data.material.map_normal = model::load_texture("res/textures/gravel_01/normal.png");
    vao_data.material.map_metal = model::load_texture("res/textures/no_metal.png");
    vao_data.material.map_rough = model::load_texture("res/textures/gravel_01/rough.png");
    vao_data.material.map_ao = model::load_texture("res/textures/gravel_01/ao.png");

    /*vao_data.material.map_albedo = model::load_texture("res/textures/ground_mud/albedo.png");
    vao_data.material.map_normal = model::load_texture("res/textures/ground_mud/normal.png");
    vao_data.material.map_metal = model::load_texture("res/textures/no_metal.png");
    vao_data.material.map_rough = model::load_texture("res/textures/ground_mud/rough.png");
    vao_data.material.map_ao = model::load_texture("res/textures/ground_mud/ao.png");*/

    /*vao_data.material.map_albedo = model::load_texture("res/textures/ground_mud_rubble/albedo.png");
    vao_data.material.map_normal = model::load_texture("res/textures/ground_mud_rubble/normal.png");
    vao_data.material.map_metal = model::load_texture("res/textures/no_metal.png");
    vao_data.material.map_rough = model::load_texture("res/textures/ground_mud_rubble/rough.png");
    vao_data.material.map_ao = model::load_texture("res/textures/ground_mud_rubble/ao.png");*/
}

void Mud_Lane::generate_vertex_data(float displacement)
{
    Lane_Heightmap height_data {nr_across, nr_points, bezier, displacement, get_width()};

    // ===| potholes |===

    //randomize pot hole placement
    for (int i = 0; i < nr_potholes; i++)
    {
        int hole_position_across {rand() % static_cast<int>(nr_across)};
        int hole_position_along {rand() % static_cast<int>(nr_points)};

        float pothole_radius {rand() / static_cast<float>(RAND_MAX) * (max_pothole_radius - min_pothole_radius) + min_pothole_radius};

        vec2 hole_pos {height_data.get_position(hole_position_across, hole_position_along)};

        if ((height_data.get_position(nr_across - 1, hole_position_along) - hole_pos).length() > pothole_radius
            && (height_data.get_position(0, hole_position_along) - hole_pos).length() > pothole_radius
            && (height_data.get_position(hole_position_across, nr_points - 1) - hole_pos).length() > pothole_radius
            && (height_data.get_position(hole_position_across, 0) - hole_pos).length() > pothole_radius)
        {
            for (int y = 0; y < nr_points; y++)
            {
                if ((height_data.get_position(hole_position_across, y) - hole_pos).length() < pothole_radius)
                {
                    for (int x = 0; x < nr_across; x++)
                    {
                        vec2 pos {height_data.get_position(x, y)};
                        if ((pos - hole_pos).length() < pothole_radius)
                        {
                            height_data.at(x, y) += (pos - hole_pos).length() - pothole_radius;
                        }
                    }
                }
            }
        }
    }

    // ===| tire tracks |===

    for (int i = 0; i < erosion_iterations; i++)
    {
        int start {rand() % static_cast<int>(nr_across - ceil(tire_width / mesh_grid_size))};
        int stop  {rand() % static_cast<int>(nr_across - ceil(tire_width / mesh_grid_size))};

        for (int index_x = 0; index_x < nr_across; index_x++)
        {
            for (int index_z = 0; index_z < nr_points; index_z++)
            {
                float start_stop_interpolation {start + (stop - start) * index_z / (nr_points - 1)};

                if (index_x > start_stop_interpolation && index_x < start_stop_interpolation + tire_width / mesh_grid_size)
                {
                    height_data.at(index_x, index_z) -= 0.01;
                }
            }
        }
    }

    // ===| lowpass the result |===

    height_data.lowpass();
    height_data.lowpass();
    height_data.lowpass();
    
    vao_data.load_buffer_data(generate_vertex_data_help(displacement, height_data));

    generate_water_surface(displacement);
}

void Mud_Lane::generate_water_surface(float displacement)
{
    Lane_Heightmap height_data {nr_across, nr_points, bezier, displacement, get_width()};

    //basic surface
    for (int y = 0; y < nr_points; y++)
    {
        for (int x = 0; x < nr_across; x++)
        {
            height_data.update_height(x, y, water_height);
        }
    }

    water_surface.load_buffer_data(generate_vertex_data_help(displacement, height_data));
    water_surface.material.map_albedo = model::load_texture("res/textures/worn_metal/albedo.png");
    water_surface.material.map_normal = model::load_texture("res/textures/worn_metal/normal.png");
    water_surface.material.map_metal = model::load_texture("res/textures/worn_metal/metal.png");
    water_surface.material.map_rough = model::load_texture("res/textures/worn_metal/rough.png");
    water_surface.material.map_ao = model::load_texture("res/textures/worn_metal/ao.png");
}

// ===============
// ===| Ditch |===
// ===============

Ditch::Ditch(float width, Bezier& bezier)
    : Lane(width, Driving_Direction::none, bezier)
{
    vao_data.material.map_albedo = model::load_texture("res/textures/grass/albedo.png");
    vao_data.material.map_normal = model::load_texture("res/textures/grass/normal.png");
    vao_data.material.map_metal = model::load_texture("res/textures/no_metal.png");
    vao_data.material.map_rough = model::load_texture("res/textures/grass/rough.png");
    vao_data.material.map_ao = model::load_texture("res/textures/grass/ao.png");
}

void Ditch::generate_vertex_data(float displacement)
{
    Lane_Heightmap height_data {nr_across, nr_points, bezier, displacement, get_width()};

    for (int y = 0; y < nr_points; y++)
    {
        height_data.update_height(0, y, 0);
        height_data.update_height(1, y, 0);
        for (int x = 2; x < nr_across; x++)
        {
            height_data.update_height(x, y, get_quadratic_height(static_cast<float>(x - 2) / (nr_across - 3)) * -get_width() / 2);
        }
    }
        vao_data.load_buffer_data(generate_vertex_data_help(displacement, height_data));
}

std::vector<model::Vao_Data> Ditch::get_lane_models(float displacement) const
{
    std::vector<model::Vao_Data> models {};
    return models;
}