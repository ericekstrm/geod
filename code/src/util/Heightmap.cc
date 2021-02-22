#include "Heightmap.h"

#include "stb_image.h"

#include <stdexcept>

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

Heightmap::Heightmap(int width, int height)
    : width {width}, height {height}
{
    heightmap_data.resize(width * height);
}

void Heightmap::set(int x, int z, float value)
{
    if (x < 0 || x >= width || z < 0 || z >= height)
    {
        throw std::out_of_range("Heightmap index out of range: [" + std::to_string(x) + ", " + std::to_string(z) + "]");
    } else
    {
        heightmap_data[x * height + z] = value;
    }
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
    return heightmap_data.at(x * height + z);
}

vec3 Heightmap::get_normal(int x, int z) const
{
    float L = get_height(x - 1, z);
    float R = get_height(x + 1, z);
    float T = get_height(x, z - 1);
    float B = get_height(x, z + 1);

    return vec3{(L - R), 2, (T - B)}.normalize(); 
}

float& Heightmap::at(int x, int z)
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
    return heightmap_data.at(x * height + z);
}

void Heightmap::lowpass()
{
    std::vector<float> new_heights {};

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            float value = 4 * get_height(x, y)
                        + 2 * get_height(x - 1, y) 
                        + 2 * get_height(x + 1, y) 
                        + 2 * get_height(x, y - 1)
                        + 2 * get_height(x, y + 1)
                        + 1 * get_height(x + 1, y + 1)
                        + 1 * get_height(x - 1, y + 1)
                        + 1 * get_height(x + 1, y - 1)
                        + 1 * get_height(x - 1, y - 1);
            new_heights.push_back(value / 16.0);
        }
    }

    heightmap_data = new_heights;
}

// ========================
// ===| Lane Heightmap |===
// ========================

Lane_Heightmap::Lane_Heightmap(int width, int length, Bezier const& bezier, float displacement, float lane_width)
    : width {width}, length {length}
{
    heightmap_data.resize(width * length);

    if (width % 2 == 0)
    {
        throw std::runtime_error("Lane width not odd.");
    }

    for (int index_z = 0; index_z < length; index_z++)
    {

        vec2 current_point {bezier.get_point(static_cast<float>(index_z) / length)};
        vec2 perpendicular_direction {bezier.get_perpendicular_direction(static_cast<float>(index_z) / length)};

        for (int index_x = 0; index_x < width; index_x++)
        {
            vec2 point {current_point + perpendicular_direction * (lane_width / (width - 1.0) * (index_x - (width - 1.0) / 2) + displacement)};
            heightmap_data[index_x * length + index_z] = std::make_pair(point, 0);
        }
    }
}

void Lane_Heightmap::set(int index_x, int index_z, vec2 const& position, float value)
{
    if (index_x < 0 || index_x >= width || index_z < 0 || index_z >= length)
    {
        throw std::out_of_range("Heightmap index out of range: [" + std::to_string(index_x) + ", " + std::to_string(index_z) + "] in Lane_Heightmap::set");
    } else
    {
        heightmap_data[index_x * length + index_z] = std::make_pair(position, value);
    }
}

void Lane_Heightmap::update_height(int index_x, int index_z, float value)
{
    if (index_x < 0 || index_x >= width || index_z < 0 || index_z >= length)
    {
        throw std::out_of_range("Heightmap index out of range: [" + std::to_string(index_x) + ", " + std::to_string(index_z) + "] in Lane_Heightmap::update_height");
    } else
    {
        heightmap_data[index_x * length + index_z].second = value;
    }
}

float Lane_Heightmap::get_height(int index_x, int index_z) const
{
    index_x = index_x % width;
    index_z = index_z % length;
    while(index_x < 0)
    {
        index_x += width;
    }
    
    while(index_z < 0)
    {
        index_z += length;
    }
    return heightmap_data.at(index_x * length + index_z).second;
}

vec2 Lane_Heightmap::get_position(int index_x, int index_z) const
{
    if (index_x < 0 || index_x >= width || index_z < 0 || index_z >= length)
    {
        throw std::out_of_range("Heightmap index out of range: [" + std::to_string(index_x) + ", " + std::to_string(index_z) + "] in Lane_Heightmap::get_position");
    }
    return heightmap_data.at(index_x * length + index_z).first;
}

float& Lane_Heightmap::at(int index_x, int index_z)
{
    index_x = index_x % width;
    index_z = index_z % length;
    while(index_x < 0)
    {
        index_x += width;
    }
    
    while(index_z < 0)
    {
        index_z += length;
    }
    return heightmap_data.at(index_x * length + index_z).second;
}

void Lane_Heightmap::lowpass()
{
    std::vector<std::pair<vec2, float>> new_heights {};

    for (int index_x = 0; index_x < width; index_x++)
    {
        for (int index_y = 0; index_y < length; index_y++)
        {
            float value = 4 * get_height(index_x, index_y)
                        + 2 * get_height(index_x - 1, index_y) 
                        + 2 * get_height(index_x + 1, index_y) 
                        + 2 * get_height(index_x, index_y - 1)
                        + 2 * get_height(index_x, index_y + 1)
                        + 1 * get_height(index_x + 1, index_y + 1)
                        + 1 * get_height(index_x - 1, index_y + 1)
                        + 1 * get_height(index_x + 1, index_y - 1)
                        + 1 * get_height(index_x - 1, index_y - 1);
            new_heights.push_back(std::make_pair(heightmap_data[index_x * length + index_y].first, value / 16.0));
        }
    }

    heightmap_data = new_heights;
}