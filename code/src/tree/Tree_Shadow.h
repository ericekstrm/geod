#pragma once

#include "tree_params.h"
#include "Vector.h"
#include "Matrix.h"
#include "Shader.h"
#include "model_util.h"

class Tree_Shadow
{
public:
    Tree_Shadow();
    ~Tree_Shadow();

    void render(mat4 const& proj_matrix, mat4 const& camera_matrix) const;

    float get_value(vec3 const& pos);
    void set_value(vec3 const& pos, float value);
    void increase_value(vec3 const& pos, float value);
    void add_node(vec3 const& pos);
    void remove_node(vec3 const& pos);

private:
    vec3 center_position;
    float* grid;
    float cell_size {0.25};

    int grid_size{512};

    vec3 to_grid_pos(vec3 const& world_position) const;
};