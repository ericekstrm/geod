#pragma once

#include "Vector.h"
#include "Bezier.h"

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

/**
 * Lane heightmaps save the heightdata for each vertex in the curved surface of the lane.
 * 
 * Each heightdata point therefor also have a position value used for normal calculation.
 * 
 */
class Lane_Heightmap
{
public:
    Lane_Heightmap(int width, int length, Bezier const& bezier, float displacement, float lane_width);

    void set(int index_x, int index_z, vec2 const& position, float value);
    void update_height(int index_x, int index_z, float value);
    float get_height(int index_x, int index_z) const;
    vec2 get_position(int index_x, int index_z) const;

    float& at(int index_x, int index_z);

    void lowpass();
    
private:
    int width;
    int length;

    //heightmap correct positions and values
    std::vector<std::pair<vec2, float>> heightmap_data;
};