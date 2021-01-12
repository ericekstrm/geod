#include "Terrain.h"
#include "stb_image.h"
#include "model_util.h"

#include <iostream>
#include <random>

Heightmap::Heightmap(std::string const& file_name)
{
    int nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(("res/terrain/heightmaps/" + file_name).c_str(), &width, &height, &nrChannels, STBI_rgb);

    if (data)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {

                unsigned bytePerPixel = 3;
                unsigned char* pixelOffset = data + (i + (width) * j) * bytePerPixel;
                unsigned char r = pixelOffset[0];
                //unsigned char g = pixelOffset[1];
                //unsigned char b = pixelOffset[2];
                //unsigned char a = nrChannels >= 4 ? pixelOffset[3] : 0xff;

                heightmap_data.push_back(static_cast<float>(r));
            }
        }
    } else
    {
        std::cout << "Failed to load heightmap: " << file_name << std::endl;
    }
    stbi_image_free(data);
}

float Heightmap::get_height(int x, int z) const
{
    x = x % width;
    z = z % height;
    while(x < 0)
    {
        x += width;
    }
    
    while(z < 0)
    {
        z += height;
    }
    return heightmap_data.at(x * width + z);
}

vec3 Heightmap::get_normal(int x, int z) const
{
    float L = get_height(x - 1, z);
    float R = get_height(x + 1, z);
    float T = get_height(x, z - 1);
    float B = get_height(x, z + 1);
    return vec3{(L - R), 2, (T - B)}.normalize(); 
}

// =================
// ===| Terrain |===
// =================

Terrain::Terrain(vec2 const& pos)
{
    position = vec3{pos.x, 0, pos.y};
    generate_terrain();
    model_data.load_buffer_data(vertices, normals, texture_coords, indices);
    model_data.material.texture_id = model::load_texture("res/terrain/textures/grass.jpg");
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

            texture_coords.push_back(static_cast<float>(i) / static_cast<float>(terrain_resolution) * 20);
            texture_coords.push_back(static_cast<float>(j) / static_cast<float>(terrain_resolution) * 20);
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

            texture_coords.push_back(static_cast<float>(i) / static_cast<float>(terrain_resolution) * 20);
            texture_coords.push_back(static_cast<float>(j) / static_cast<float>(terrain_resolution) * 20);
        }
    }
}

void Terrain::generate_perlin_terrain()
{
    //int num_octaves;    //number of noise functions of different density, more octaves means higher level of detail
    //float smoothness;   //between 0-1
    //int seed;           //for the random part, (same seed=same output)


    for (int i = -terrain_resolution / 2.0; i < terrain_resolution / 2.0; i++)
    {
        for (int j = -terrain_resolution / 2.0; j < terrain_resolution / 2.0; j++)
        {
            float height {get_perlin_height(i, j)};
            vertices.push_back(terrain_size / static_cast<float>(terrain_resolution) * i);
            vertices.push_back(height);
            vertices.push_back(terrain_size / static_cast<float>(terrain_resolution) * j);

            normals.push_back(0);
            normals.push_back(1);
            normals.push_back(0);

            texture_coords.push_back(static_cast<float>(i) / static_cast<float>(terrain_resolution) * 20);
            texture_coords.push_back(static_cast<float>(j) / static_cast<float>(terrain_resolution) * 20);
        }
    }
}

float Terrain::get_perlin_height(int x, int z) const
{
    float total_height {0};
    float frequency {1.0};
    float smoothness {0.5};

    for (int i = 0; i < 1; i++)
    {
        float amplitude {static_cast<float>(pow((1 - smoothness), i))};

        total_height += amplitude * generate_interpolated_height(x * frequency, z * frequency);

        frequency *= 2;
    }
    
    return total_height;
}

float Terrain::generate_perlin_noise(int x, int z) const
{
    srand(x + z * terrain_size);
    return ((float) rand() / (float) RAND_MAX);
}

float Terrain::generate_interpolated_height(float x, float z) const
{
    int int_x = static_cast<int>(x);
    int int_z = static_cast<int>(z);
    float fraction_x = x - int_x;
    float fraction_z = z - int_z;

    float ul = generate_perlin_noise(int_x,     int_z);
    float ur = generate_perlin_noise(int_x + 1, int_z);
    float dl = generate_perlin_noise(int_x,     int_z + 1);
    float dr = generate_perlin_noise(int_x + 1, int_z + 1);

    float h1 = interpolate(ul, ur, fraction_x);
    float h2 = interpolate(dl, dr, fraction_x);

    return interpolate(h1, h2, fraction_z);
}

float Terrain::interpolate(float p1, float p2, float blend) const
{
    float new_blend = (1.0 - cos(blend * 3.1415)) * 0.5; //value between 0 and 1
    return p1 * (1 - new_blend) + p2 * new_blend; 
}
