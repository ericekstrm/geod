#pragma once

#include "Shader.h"
#include "Light.h"

class Model_Shader : public Shader
{
public:
    Model_Shader();
    Model_Shader(std::string const& vertex_file, std::string const& fragment_file);
    virtual ~Model_Shader();

    void load_model_matrix(Matrix4 const& mat) const;

    void load_camera_position  (vec3 const& camera_pos) const;
    void load_light_space_matrix(vec3 const& light_pos) const;
    void load_lights(Light_Container const& light_container) const;
    void load_material_properties(model::Material const& mat) const;
};
