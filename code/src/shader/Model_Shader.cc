#include "Model_Shader.h"

#include "settings.h"

Model_Shader::Model_Shader()
    : Model_Shader{"model.vert", "model.frag"}
{
}

Model_Shader::Model_Shader(std::string const& vertex_file, std::string const& fragment_file)
    : Shader{vertex_file, fragment_file}
{
}

Model_Shader::~Model_Shader()
{
}

void Model_Shader::load_model_matrix(Matrix4 const& mat) const
{
    load_mat4("model_matrix", mat);
}

void Model_Shader::load_camera_position(vec3 const& camera_pos) const
{
    load_vec3("camera_pos", camera_pos);
}

void Model_Shader::load_light_space_matrix(vec3 const& light_pos) const
{
    vec3 center_pos {0,0,0};
    mat4 light_view {look_at(light_pos, center_pos, vec3{0, 1, 0})};

    load_mat4("light_space_matrix", light_projection * light_view);
}

void Model_Shader::load_lights(Light_Container const& light_container) const
{
    load_vec3_arr("light_pos_dir", light_container.get_pos_dir_data());
    load_vec3_arr("light_color", light_container.get_color_data());
    load_vec3_arr("light_attenuation_params", light_container.get_attenuation_data());
    load_bool_arr("light_type", light_container.get_light_type_data());
    load_int("number_of_lights", light_container.get_number_of_lights());

    /*std::cout << "-----------------" << std::endl;
    std::cout << light_container.get_number_of_lights() << std::endl;
    for(vec3 v : light_container.get_pos_dir_data())
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    for(vec3 v : light_container.get_color_data())
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    for(vec3 v : light_container.get_attenuation_data())
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    for(int v : light_container.get_light_type_data())
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;*/
}

void Model_Shader::load_material_properties(model::Material const& mat) const
{
    load_vec3("ka", mat.ka);
    load_vec3("kd", mat.kd);
    load_vec3("ks", mat.ks);
    load_float("specular_exponent", mat.a);
    load_bool("use_specularity_map", mat.use_specularity_map);
}