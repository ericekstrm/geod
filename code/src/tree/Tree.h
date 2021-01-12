#pragma once

#include <string>
#include <vector>

#include "Vector.h"
#include "Model.h"
#include "Tree_Shadow.h"
#include "Model_Instance_Shader.h"
#include "Camera.h"
#include "Light.h"

class Node;

class Tree
{
public:
    Tree(vec3 const& position = vec3{0, 0, 0});
    ~Tree();

    Tree(Tree const&) = delete;
    Tree(Tree && rhs);
    Tree& operator=(Tree const&) = delete;
    Tree& operator=(Tree && rhs);

    void render(Model_Shader const * shader) const;
    void render_leafs(Camera const * camera, Light_Container const * lights) const;
    void render_leafs(Model_Instance_Shader const * shader) const;
    void update(float delta_time);

    void grow();

private:

    //tmp function
    void generate();

    // Growth functions
    float calc_light_res(int current_time);

    // Model creation
    void create_buffer_data();
    void recreate_buffer_data();
    model::Buffer_Data data {};
    model::Vao_Data vao_data;

    model::Vao_Data leaf_vao;
    std::vector<mat4> leaf_transforms {};
    Model_Instance_Shader leaf_shader {};
    unsigned int leaf_buffer;

    Node* root {nullptr};

    float light_res {};

    int age {0};

    vec3 position;
};

class Node
{
public:
    Node();
    Node(vec3 const& direction, vec3 parent_position, int current_time);
    ~Node();

    // Growth functions
    float calc_borchert_light_res(int current_time);
    void calc_borchert_honda(float recieved_light_res, int current_time);

    void shoot_main(int nr_new_nodes, int current_time);
    void shoot_lateral(int nr_new_nodes, int current_time);
    void shoot(int nr_new_nodes, int current_time);

    float calc_branch_radii();
    void calc_shedding_branches();
    vec3 calc_optimal_growth_direction() const;

    void shed(Node* & branch);

    float get_light_res() const { return main_light_res + lateral_light_res; }
    int get_supporting_terminals() const;

    std::string to_string() const;
    void create_buffer_data(model::Buffer_Data & data, vec3 const& parent_position) const;
    void create_leaf_buffer_data(std::vector<mat4> & transforms) const;

private:

    // Internode
    float radius {tree::default_radius};
    vec3 direction;
    float length {tree::default_length};

    // Dont really want to have this here, but needed for filling in the shadow-map.
    vec3 position;

    // Growth res (borchert-honda)
    int creation_time; //the year the node was created
    float main_light_res {}; // The generated light resource from main_branch node.
    float lateral_light_res {}; // The generated light resource from lateral branch node.

    bool has_apical_bud() const { return main_branch == nullptr; }
    bool has_lateral_bud() const { return lateral_branch == nullptr; }
    bool has_main_branch() const { return main_branch != nullptr; }
    bool has_lateral_branch() const { return lateral_branch != nullptr; }

    Node* main_branch {nullptr};
    Node* lateral_branch {nullptr};

public:
    static Tree_Shadow tree_shadow;
};