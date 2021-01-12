#include "Tree.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <array>
#include <numeric>

#include "model_util.h"
#include "Matrix.h"

Tree_Shadow Node::tree_shadow = {};

Tree::Tree(vec3 const& in_position)
    : position {in_position}
{
    leaf_vao = model::load_model_from_file("leaf1");
    generate();
}

Tree::~Tree()
{
    delete root;
}

Tree::Tree(Tree && rhs)
{
    *this = std::move(rhs);
}

Tree& Tree::operator=(Tree && rhs)
{

    //self assignment?

    std::swap(root, rhs.root);

    std::swap(data, rhs.data);
    std::swap(vao_data, rhs.vao_data);
    
    std::swap(leaf_vao, rhs.leaf_vao);
    std::swap(leaf_transforms, rhs.leaf_transforms);
    std::swap(leaf_shader, rhs.leaf_shader);
    std::swap(leaf_buffer, rhs.leaf_buffer);

    light_res = rhs.light_res;
    age = rhs.age;
    position = rhs.position;
    
    return *this;
}

void Tree::render(Model_Shader const * shader) const
{
    glBindVertexArray(vao_data.vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vao_data.material.texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    shader->load_model_matrix(translation_matrix(position.x, position.y, position.z));
    shader->load_material_properties(vao_data.material);

    glDrawElements(GL_TRIANGLES, vao_data.indices_count, GL_UNSIGNED_INT, 0);
}

void Tree::render_leafs(Camera const * camera, Light_Container const * lights) const
{
    glBindVertexArray(leaf_vao.vao);

    leaf_shader.start();
    leaf_shader.load_camera_matrix(camera->get_camera_matrix());
    leaf_shader.load_camera_position(camera->get_position());
    //leaf_shader.load_light_space_matrix();
    leaf_shader.load_lights(*lights);
    
    render_leafs(&leaf_shader);

    leaf_shader.stop();
}

void Tree::render_leafs(Model_Instance_Shader const * shader) const
{
    glBindVertexArray(leaf_vao.vao);

    shader->load_material_properties(leaf_vao.material);
    shader->load_projection_matrix();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, leaf_vao.material.texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDrawElementsInstanced(GL_TRIANGLES, leaf_vao.indices_count, GL_UNSIGNED_INT, 0, leaf_transforms.size());
}

void Tree::update(float)
{
    grow();
    recreate_buffer_data();
}

void Tree::generate()
{
    //create trunk
    root = new Node(vec3{0,1,0}, position, 0);
    root->shoot_main(3, 0);

    for(int i = 0; i < 1; i++)
    {
        grow();
    }

    create_buffer_data();
}

float Tree::calc_light_res(int current_time)
{
    light_res = tree::alpha * root->calc_borchert_light_res(current_time);
    return light_res;
}

void Tree::grow()
{
    age++;
    calc_light_res(age);
    root->calc_borchert_honda(light_res, age);
    root->calc_branch_radii();
    root->calc_shedding_branches(); //switch place with branch radius
}

void Tree::create_buffer_data()
{
    root->create_buffer_data(data, vec3{0,0,0});
    vao_data.load_buffer_data(data);
    vao_data.material.texture_id = model::load_texture("res/tree/oak_texture/Wood_Bark_006_basecolor.jpg");
    vao_data.material.kd = vec3{1,1,1};
    vao_data.material.ka = vec3{1,1,1};
    vao_data.material.ks = vec3{1,1,1};
    vao_data.material.a = 225;

    glGenBuffers(1, &leaf_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, leaf_buffer);
    root->create_leaf_buffer_data(leaf_transforms);
    glBufferData(GL_ARRAY_BUFFER, leaf_transforms.size() * sizeof(mat4), &leaf_transforms[0], GL_STATIC_DRAW);

    glBindVertexArray(leaf_vao.vao);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(2 * sizeof(vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(3 * sizeof(vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void Tree::recreate_buffer_data()
{
    data.vertices.clear();
    data.normals.clear();
    data.texture_coords.clear();
    data.indices.clear();
    leaf_transforms.clear();

    root->create_buffer_data(data, vec3{0,0,0});
    vao_data.reload_buffer_data(data);

    root->create_leaf_buffer_data(leaf_transforms);
    glBindBuffer(GL_ARRAY_BUFFER, leaf_buffer);
    glBufferData(GL_ARRAY_BUFFER, leaf_transforms.size() * sizeof(mat4), &leaf_transforms[0], GL_STATIC_DRAW);
}

// ==============
// ===| Node |===
// ==============

Node::Node()
{
}

Node::Node(vec3 const& direction, vec3 parent_position, int current_time)
    : direction {direction}, position {parent_position + direction * length}, creation_time {current_time}
{
    tree_shadow.add_node(position);
}

Node::~Node()
{
    tree_shadow.remove_node(position);

    delete main_branch;
    delete lateral_branch;
}

float Node::calc_borchert_light_res(int current_time)
{
    main_light_res = 0;
    lateral_light_res = 0;

    // calc res from branches
    if (main_branch != nullptr)
    {
        main_light_res = main_branch->calc_borchert_light_res(current_time);
    }
    else
    {
        // calc res for apical bud
        float light_exposure {std::max(tree::full_exposure_light - tree_shadow.get_value(position) + tree::a, 0.0f)};
        main_light_res = pow(light_exposure, pow(current_time - creation_time, tree::sigma));
    }

    if (lateral_branch != nullptr)
    {
        lateral_light_res += lateral_branch->calc_borchert_light_res(current_time);
    }
    else if (main_branch != nullptr)
    {
        // calc res for lateral bud
        float light_exposure {std::max(tree::full_exposure_light - tree_shadow.get_value(position) + tree::a, 0.0f)};
        lateral_light_res = pow(light_exposure, pow(current_time - creation_time, tree::sigma));
    }
    
    return main_light_res + lateral_light_res - tree::BM * pow(radius / tree::default_radius, tree::BE);
}

void Node::calc_borchert_honda(float recieved_light_recources, int current_time)
{
    float vm = recieved_light_recources * ((tree::lambda * main_light_res) / (tree::lambda * main_light_res + (1 - tree::lambda) * lateral_light_res));
    float vl = recieved_light_recources * (((1 - tree::lambda) * lateral_light_res) / (tree::lambda * main_light_res + (1 - tree::lambda) * lateral_light_res));

    if (main_branch != nullptr && vm > 0)
    {
        main_branch->calc_borchert_honda(vm, current_time);
    }
    else
    {
        int nr_new_nodes = static_cast<int>(std::floor(vm));

        shoot_main(nr_new_nodes, current_time);
    }

    if (lateral_branch != nullptr && vl > 0)
    {
        lateral_branch->calc_borchert_honda(vl, current_time);
    }
    else
    {
        int nr_new_nodes = static_cast<int>(std::floor(vl));

        shoot_lateral(nr_new_nodes, current_time);
    }
}

void Node::shoot_main(int nr_new_nodes, int current_time)
{
    if (nr_new_nodes > 0)
    {
        main_branch = new Node(direction, position, current_time);
        main_branch->shoot(--nr_new_nodes, current_time);
    }
}

void Node::shoot_lateral(int nr_new_nodes, int current_time)
{
    if (nr_new_nodes > 0)
    {
        vec3 random_vector {(float) rand() / RAND_MAX - 0.5f, (float)rand() / RAND_MAX - 0.5f, (float)rand() / RAND_MAX - 0.5f};
        vec3 shifted_direction {rotation_matrix( 30, random_vector) * direction};
        lateral_branch = new Node(shifted_direction, position, current_time);
        lateral_branch->shoot(--nr_new_nodes, current_time);
    }
}

void Node::shoot(int nr_new_nodes, int current_time)
{
    if (nr_new_nodes > 0)
    {
        vec3 optimal_direction = (direction * 0.8f + calc_optimal_growth_direction() * 0.2f + vec3{0,-1,0} * tree::tropism).normalize();
        main_branch = new Node(optimal_direction, position, current_time);
        main_branch->shoot(--nr_new_nodes, current_time);
    }
}

float Node::calc_branch_radii()
{
    float tmp {};
    if (has_lateral_branch())
    {
        tmp += pow(lateral_branch->calc_branch_radii(), 2);
    }
    if (has_main_branch())
    {
        tmp += pow(main_branch->calc_branch_radii(), 2);
    }

    float new_radius = static_cast<float>(sqrt(tmp));
    if (new_radius > radius)
    {
        radius = new_radius;
    }
    return radius;
}

void Node::calc_shedding_branches()
{
    if (has_main_branch())
    {
        if (main_light_res < 0 && main_branch->get_supporting_terminals() < tree::Nmin)
        {
            shed(main_branch);
        } else 
        {
            main_branch->calc_shedding_branches();
        }
    }

    if (has_lateral_branch())
    {
        if (lateral_light_res < 0 && lateral_branch->get_supporting_terminals() < tree::Nmin)
        {
            shed(lateral_branch);
        } else 
        {
            lateral_branch->calc_shedding_branches();
        }
    }
}

vec3 Node::calc_optimal_growth_direction() const
{
    std::vector<vec3> neighbours
    {
        vec3{-1,-1,-1}, vec3{-1,-1, 0}, vec3{-1,-1, 1}, 
        vec3{-1, 0,-1}, vec3{-1, 0, 0}, vec3{-1, 0, 1}, 
        vec3{-1, 1,-1}, vec3{-1, 1, 0}, vec3{-1, 1, 1}, 
        vec3{ 0,-1,-1}, vec3{ 0,-1, 0}, vec3{ 0,-1, 1}, 
        vec3{ 0, 0,-1}, vec3{ 0, 0, 0}, vec3{ 0, 0, 1}, 
        vec3{ 0, 1,-1}, vec3{ 0, 1, 0}, vec3{ 0, 1, 1}, 
        vec3{ 1,-1,-1}, vec3{ 1,-1, 0}, vec3{ 1,-1, 1}, 
        vec3{ 1, 0,-1}, vec3{ 1, 0, 0}, vec3{ 1, 0, 1}, 
        vec3{ 1, 1,-1}, vec3{ 1, 1, 0}, vec3{ 1, 1, 1}
    };

    std::vector<vec3> optimal_direction {};
    float min_shadow {10000};
    for (size_t i = 0; i < neighbours.size(); i++)
    {
        float shadow {tree_shadow.get_value(position + neighbours[i])};
        if (shadow < min_shadow)
        {
            min_shadow = shadow;
            optimal_direction.clear();
            optimal_direction.push_back(neighbours[i]);
        } else if (shadow == min_shadow)
        {
            optimal_direction.push_back(neighbours[i]);
        }
    }

    return std::accumulate(optimal_direction.begin(), optimal_direction.end(), vec3{}).normalize();
}

void Node::shed(Node* & branch)
{
    delete branch;
    branch = nullptr;
}

std::string Node::to_string() const
{
    std::stringstream output {};

    output << "[lateral: ";
    if (lateral_branch != nullptr)
    {
        output << lateral_branch->to_string();
    }
    output << ", main: ";
    if (main_branch != nullptr)
    {
        output << main_branch->to_string();
    }
    output << "]";
    return output.str();
}

int Node::get_supporting_terminals() const
{
    int sum {0};
    if (has_lateral_branch())
    {
        sum += lateral_branch->get_supporting_terminals();
    } else
    {
        sum++;
    }
    if (has_main_branch())
    {
        sum += main_branch->get_supporting_terminals();
    } else
    {
        sum++;
    }
    return sum;
}

void Node::create_buffer_data(model::Buffer_Data & data, vec3 const& parent_position) const
{

    int nr_points = 10;
    int nr_previously_added_vertices = data.vertices.size() / 3;

    // == Add Vertex and Texture Data ==

    //lower surounding points
    float radians_per_point = 360 / nr_points;
    mat4 rot_mat {rotation_matrix(radians_per_point, direction)};
    vec3 perpendicular_dir = direction.cross(direction.y != 0 ? vec3{1,0,0} : vec3{0,1,0}).normalize();
    vec3 point = parent_position + perpendicular_dir * radius;
    vec3 normal = perpendicular_dir;
    for (int i = 0; i < nr_points + 1; i++)
    {
        point = rot_mat * (point - parent_position) + parent_position;
        normal = rot_mat * normal;

        data.vertices.push_back(point.x);
        data.vertices.push_back(point.y);
        data.vertices.push_back(point.z);

        data.texture_coords.push_back(i / static_cast<float>(nr_points));
        data.texture_coords.push_back(0);

        data.normals.push_back(normal.x);
        data.normals.push_back(normal.y);
        data.normals.push_back(normal.z);
    }

    //upper midpoint
    vec3 current_node_postion {parent_position + direction * length};

    //upper surounding points
    if (has_main_branch() && has_lateral_branch())
    {
        point = current_node_postion + perpendicular_dir * std::max(main_branch->radius, lateral_branch->radius);
    } else if (has_main_branch())
    {
        point = current_node_postion + perpendicular_dir * main_branch->radius;
    } else
    {
        point = current_node_postion + perpendicular_dir * radius;
    }
    normal = perpendicular_dir;
    
    for (int i = 0; i < nr_points + 1; i++)
    {
        point = rot_mat * (point - current_node_postion) + current_node_postion;
        normal = rot_mat * normal;

        data.vertices.push_back(point.x);
        data.vertices.push_back(point.y);
        data.vertices.push_back(point.z);

        data.texture_coords.push_back(i / static_cast<float>(nr_points));
        data.texture_coords.push_back(1);
        
        data.normals.push_back(normal.x);
        data.normals.push_back(normal.y);
        data.normals.push_back(normal.z);
    }

    // == Add Indices Data ==

    for (int i = 0; i < nr_points; i++)
    {
        float br = i; // first point is the middle
        float bl = br + 1; // next in the circle
        float tr = br + nr_points + 1; // 1 lap later + the top middle point 
        float tl = tr + 1; // next in the circle

        data.indices.push_back(nr_previously_added_vertices + bl);
        data.indices.push_back(nr_previously_added_vertices + tr);
        data.indices.push_back(nr_previously_added_vertices + tl);

        data.indices.push_back(nr_previously_added_vertices + bl);
        data.indices.push_back(nr_previously_added_vertices + br);
        data.indices.push_back(nr_previously_added_vertices + tr);
    }

    // == Add Child Data == 
    if (lateral_branch != nullptr)
    {
        lateral_branch->create_buffer_data(data, current_node_postion);
    }
    
    if (main_branch != nullptr)
    {
        main_branch->create_buffer_data(data, current_node_postion);
    }
}

void Node::create_leaf_buffer_data(std::vector<mat4> & transforms) const
{
    if (!has_main_branch() || !has_lateral_branch())
    {

        vec3 random_vector {(float) rand() / RAND_MAX - 0.5f, (float)rand() / RAND_MAX - 0.5f, (float)rand() / RAND_MAX - 0.5f};
        vec3 leaf_direction {rotation_matrix( 30, random_vector) * direction};

        mat4 rotation {rotation_matrix(vec3{0, 1, 0}, leaf_direction) * rotation_matrix(rand() % 180, vec3{0, 1, 0})};

        float scale = 2;

        transforms.push_back((translation_matrix(position) * rotation * scale_matrix(scale, scale, scale)).transpose());
    }

    if (has_lateral_branch())
    {
        lateral_branch->create_leaf_buffer_data(transforms);
    }

    if (has_main_branch())
    {
        main_branch->create_leaf_buffer_data(transforms);
    }
}