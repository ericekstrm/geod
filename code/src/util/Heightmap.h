#pragma once

#include "Vector.h"

#include <string>

class Heightmap
{
public:
    Heightmap(std::string const& file_name);
    Heightmap(int width, int height);

    void set(int x, int z, float value);
    float get_height(int x, int z) const;
    vec3 get_normal(int x, int z) const;

    float& at(int x, int z);

    /**
     * Preforms lowpass filtering of the heightmap with 
     * a 3x3 gaussian filter.
     */
    void lowpass();
    
private:
    int width;
    int height;
    std::vector<float> heightmap_data;
};