#pragma once

#include "Model.h"
#include "settings.h"
#include "Heightmap.h"

class Terrain
{
public:
    Terrain();

private:

    void generate_terrain();
    void set_indices();

    //terrain generation functions
    void flat_terrain();
    void heightmap_terrain(std::string const& file_name);

    model::Vao_Data model_data {};
    model::Buffer_Data buffer_data {};

    // Terrain parameters
    int terrain_size {512};       // Size that the texture "tile" will take up in the world.
    int terrain_resolution {512}; // Number of vertices in each dimension of the texture "tile".
    float terrain_max_height {20};
};
