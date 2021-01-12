#include "Tree_Shadow.h"

#include <iostream>
#include "Matrix.h"

#ifdef WIN32
    #include "glad/glad.h"
#else
    #include "GL/gl.h"
#endif

Tree_Shadow::Tree_Shadow()
{
    grid = new float[grid_size * grid_size * grid_size];
    for (int i = 0; i < grid_size * grid_size * grid_size; i++)
    {
        grid[i] = 0;
    }
}

Tree_Shadow::~Tree_Shadow()
{
    delete[] grid;
}

float Tree_Shadow::get_value(vec3 const& pos)
{
    vec3 grid_pos = to_grid_pos(pos);
    grid_pos.x = roundf(grid_pos.x);
    grid_pos.y = roundf(grid_pos.y);
    grid_pos.z = roundf(grid_pos.z);

    if (grid_pos.x < 0 || grid_pos.y < 0 || grid_pos.z < 0 || 
        grid_pos.x >= grid_size || grid_pos.y >= grid_size || grid_pos.z >= grid_size)
    {
        //outside the shadow_box. assume there is no light here
        return 100000;
    }
    return grid[static_cast<int>(grid_pos.x + grid_pos.y * grid_size + grid_pos.z * grid_size * grid_size)];
}

void Tree_Shadow::set_value(vec3 const& pos, float value)
{
    vec3 grid_pos = to_grid_pos(pos);
    grid_pos.x = roundf(grid_pos.x);
    grid_pos.y = roundf(grid_pos.y);
    grid_pos.z = roundf(grid_pos.z);

    if (grid_pos.x < 0 || grid_pos.y < 0 || grid_pos.z < 0 || 
        grid_pos.x >= grid_size || grid_pos.y >= grid_size || grid_pos.z >= grid_size)
    {
        //outside the shadow_box. cant change value here
        return;
    }

    grid[static_cast<int>(grid_pos.x + grid_pos.y * grid_size + grid_pos.z * grid_size * grid_size)] = value;
}

void Tree_Shadow::increase_value(vec3 const& pos, float value)
{
    vec3 grid_pos = to_grid_pos(pos);
    grid_pos.x = roundf(grid_pos.x);
    grid_pos.y = roundf(grid_pos.y);
    grid_pos.z = roundf(grid_pos.z);

    if (grid_pos.x < 0 || grid_pos.y < 0 || grid_pos.z < 0 || 
        grid_pos.x >= grid_size || grid_pos.y >= grid_size || grid_pos.z >= grid_size)
    {
        //outside the shadow_box. cant change value here
        return;
    }

    grid[static_cast<int>(grid_pos.x + grid_pos.y * grid_size + grid_pos.z * grid_size * grid_size)] += value;
}

void Tree_Shadow::add_node(vec3 const& pos)
{
    vec3 grid_pos = to_grid_pos(pos);
    int node_x = roundf(grid_pos.x);
    int node_y = roundf(grid_pos.y);
    int node_z = roundf(grid_pos.z);

    int lower_limit = 10;
    if ( node_y - lower_limit < 0)
    {
        lower_limit = node_y;
    }

    for (int i = 0; i < lower_limit; i++)
    {
        for (int j = -i; j <= i; j++)
        {
            for (int k = -i; k <= i; k++)
            {
                int I = node_x + j;
                int J = node_y - i;
                int K = node_z + k;

                if (I < 0 || J < 0 || K < 0 || 
                    I >= grid_size || J >= grid_size || K >= grid_size)
                {
                    //outside the shadow_box. cant change value here
                    continue;
                }

                grid[I + J * grid_size + K * grid_size * grid_size] += tree::a * pow(tree::b, -i);
            }
        }
    }
}

void Tree_Shadow::remove_node(vec3 const& pos)
{
    vec3 grid_pos = to_grid_pos(pos);
    int node_x = roundf(grid_pos.x);
    int node_y = roundf(grid_pos.y);
    int node_z = roundf(grid_pos.z);

    int lower_limit = 10;
    if ( node_y - lower_limit < 0)
    {
        lower_limit = node_y;
    }

    for (int i = 0; i < lower_limit; i++)
    {
        for (int j = -i; j <= i; j++)
        {
            for (int k = -i; k <= i; k++)
            {
                int I = node_x + j;
                int J = node_y - i;
                int K = node_z + k;

                if (I < 0 || J < 0 || K < 0 || 
                    I >= grid_size || J >= grid_size || K >= grid_size)
                {
                    //outside the shadow_box. cant change value here
                    continue;
                }

                grid[I + J * grid_size + K * grid_size * grid_size] -= tree::a * pow(tree::b, -i);
            }
        }
    }
}

vec3 Tree_Shadow::to_grid_pos(vec3 const& world_position) const
{
    float grid_space_x = roundf(world_position.x / cell_size + grid_size / 2.0);
    float grid_space_y = roundf(world_position.y / cell_size);
    float grid_space_z = roundf(world_position.z / cell_size + grid_size / 2.0);

    return vec3{grid_space_x, grid_space_y, grid_space_z};
}