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