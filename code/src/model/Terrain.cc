#include "Terrain.h"
#include "stb_image.h"
#include "model_util.h"

#include <iostream>
#include <random>

Terrain::Terrain(vec2 const& pos)
{
    position = vec3{pos.x, 0, pos.y};
    generate_terrain();
    model_data.load_buffer_data(vertices, normals, texture_coords, tangents, indices);
    model_data.material.map_albedo = model::load_texture("res/textures/grass/albedo.png");
    model_data.material.map_normal = model::load_texture("res/textures/grass/normal.png");
    model_data.material.map_metal = model::load_texture("res/textures/no_metal.png");
    model_data.material.map_rough = model::load_texture("res/textures/grass/rough.png");
    model_data.material.map_ao = model::load_texture("res/textures/grass/ao.png");
}

Terrain::~Terrain()
{
}

void Terrain::generate_terrain()
{
    //heightmap_terrain("island3.png");
    flat_terrain();
    //generate_perlin_terrain();

    set_indices();
}

void Terrain::set_indices()
{
    for (int i = 0; i < terrain_resolution; i++)
    {
        for (int j = 0; j < terrain_resolution; j++)
        {
            int topLeft = i * (terrain_resolution + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (terrain_resolution + 1) + j;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(topLeft);
            indices.push_back(bottomRight);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
        }
    }
}

void Terrain::flat_terrain()
{
    for (int i = -terrain_resolution / 2.0; i < terrain_resolution / 2.0 + 1; i++)
    {
        for (int j = -terrain_resolution / 2.0; j < terrain_resolution / 2.0 + 1; j++)
        {
            float r = 0;
            vertices.push_back(terrain_size / static_cast<float>(terrain_resolution) * i);
            vertices.push_back(r);
            vertices.push_back(terrain_size / static_cast<float>(terrain_resolution) * j);

            normals.push_back(0);
            normals.push_back(1);
            normals.push_back(0);
            tangents.push_back(1);
            tangents.push_back(0);
            tangents.push_back(0);

            texture_coords.push_back(static_cast<float>(i) / static_cast<float>(terrain_resolution) * 100);
            texture_coords.push_back(static_cast<float>(j) / static_cast<float>(terrain_resolution) * 100);
        }
    }
}

void Terrain::heightmap_terrain(std::string const& file_name)
{
    Heightmap hm {file_name};

    for (int i = -terrain_resolution / 2.0; i < terrain_resolution / 2.0; i++)
    {
        for (int j = -terrain_resolution / 2.0; j < terrain_resolution / 2.0; j++)
        {
            float r = hm.get_height(i, j) / 256;
            vertices.push_back(terrain_size / static_cast<float>(terrain_resolution) * i);
            vertices.push_back((r - 0.5) * terrain_max_height);
            vertices.push_back(terrain_size / static_cast<float>(terrain_resolution) * j);

            vec3 normal {hm.get_normal(i, j)}; 
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
            tangents.push_back(1);
            tangents.push_back(0);
            tangents.push_back(0);

            texture_coords.push_back(static_cast<float>(i) / static_cast<float>(terrain_resolution) * 20);
            texture_coords.push_back(static_cast<float>(j) / static_cast<float>(terrain_resolution) * 20);
        }
    }
}
