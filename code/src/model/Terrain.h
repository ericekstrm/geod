#pragma once

#include "Model.h"
#include "settings.h"

class Heightmap
{
public:
    Heightmap(std::string const& file_name);
    float get_height(int x, int z) const;
    vec3 get_normal(int x, int z) const;
    
private:
    int width;
    int height;
    std::vector<float> heightmap_data {};
};

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
    void generate_perlin_terrain();

    //perlin noise functions
    float get_perlin_height(int x, int z) const;
    float generate_perlin_noise(int x, int z) const;
    float generate_interpolated_height(float x, float z) const;
    float interpolate(float p1, float p2, float blend) const;

    std::vector<float> vertices = {};
    std::vector<float> normals {};
    std::vector<float> texture_coords = {};
    std::vector<int> indices = {};
};
