#pragma once

#include "Model.h"
#include "settings.h"
#include "Heightmap.h"

class Terrain : public Model
{
public:
    Terrain(vec2 const& pos = {0,0});
    ~Terrain();

private:

    void generate_terrain();
    void set_indices();

    //terrain generation functions
    void flat_terrain();
    void heightmap_terrain(std::string const& file_name);

    std::vector<float> vertices = {};
    std::vector<float> normals {};
    std::vector<float> texture_coords = {};
    std::vector<float> tangents = {};
    std::vector<int> indices = {};

    // Terrain parameters
    int terrain_size {512};       // Size that the texture "tile" will take up in the world.
    int terrain_resolution {512}; // Number of vertices in each dimension of the texture "tile".
    float terrain_max_height {20};
};
