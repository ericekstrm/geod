#include "Terrain_Container.h"

Terrain_Container::Terrain_Container()
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            add_tile(i, j);
        }   
    }
}

void task1(std::string msg)
{
    std::cout << "task1 says: " << msg << std::endl;
}

void Terrain_Container::update(vec3 const& camera_pos)
{
    if ((prev_camera_pos - camera_pos).length() > terrain_size / 2)
    {
        int x {(static_cast<int>(camera_pos.x) - terrain_size / 2) / terrain_size};
        int y {(static_cast<int>(camera_pos.z) - terrain_size / 2) / terrain_size};
        if (camera_pos.x > 0)
        {
            x++;
        }
        if (camera_pos.z > 0)
        {
            y++;
        }
        for (int i = x - 1; i <= x + 1; i++)
        {
            for (int j = y - 1; j <= y + 1; j++)
            {
                tiles_to_add.push(vec2{static_cast<float>(i), static_cast<float>(j)});
            }   
        }

        prev_camera_pos = camera_pos;
    }

    if (!tiles_to_add.empty())
    {
        vec2 tile {tiles_to_add.front()};
        tiles_to_add.pop();
        add_tile(tile.x, tile.y);
    }
}

void Terrain_Container::render(Model_Shader const* shader) const
{
    for (auto it = terrain_tiles.begin(); it != terrain_tiles.end(); it++)
    {
        //if ()
        it->second.render(shader);
    }
}

//adds a tile if it does not already exist.
void Terrain_Container::add_tile(float x, float z)
{
    terrain_tiles.insert(std::pair<vec2, Terrain>(vec2{x, z}, Terrain{vec2{x * terrain_size, z * terrain_size + 5}}));
}