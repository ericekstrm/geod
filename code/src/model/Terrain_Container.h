#pragma once

#include <map>
#include <queue>

#include "Terrain.h"

class Terrain_Container
{
public:

    Terrain_Container();
    

    void update(vec3 const& camera_pos);
    void render(Model_Shader const* shader) const;

    void add_tile(float x, float z);

private:
    std::queue<vec2> tiles_to_add {};
    std::map<vec2, Terrain> terrain_tiles {};

    vec3 prev_camera_pos {};
};