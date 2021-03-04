#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "Matrix.h"
#include "model_util.h"
#include "Light.h"

class Shader
{
public:
    Shader(std::string const& vertex_file, std::string const& fragment_file);
    virtual ~Shader();

    void start() const;
    void stop() const;

    int get_programID() const;

    void load_projection_matrix() const;
    void load_camera_matrix(Matrix4 const& mat) const;
    void load_color(vec3 const& color) const;
    void load_position(vec3 const& pos) const;
    void load_position(vec2 const& pos) const;
    void load_model_matrix(Matrix4 const& model_matrix) const;
    void load_camera_position  (vec3 const& camera_pos) const;
    void load_sun_pos(vec2 const& sun_pos) const;
    void load_light_space_matrix(vec3 const& light_pos) const;
    void load_lights(Light_Container const& light_container) const;
    void load_material_properties(model::Material const& mat) const;
    void load_clipping_plane(vec4 const& plane) const;
    void load_has_opacity_map(bool b) const;

    void load_instance_transforms(std::vector<mat4> const& transforms) const;

    //For text rendering
    void load_font_color(vec3 const& color) const;
    void load_text_pos_matrix(Matrix4 const& mat) const;
    void load_char_pos_matrix(Matrix4 const& mat) const;

protected:
    int get_uniform_location(std::string const& uniform_name) const;

    void load_int  (std::string const& name, int         value) const;
    void load_float(std::string const& name, float       value) const;
    void load_bool (std::string const& name, bool        value) const;
    void load_mat4 (std::string const& name, mat4 const& value) const;
    void load_vec2 (std::string const& name, vec2 const& value) const;
    void load_vec3 (std::string const& name, vec3 const& value) const;
    void load_vec4 (std::string const& name, vec4 const& value) const;
    
    void load_vec3_arr(std::string const& name, std::vector<vec3> const& value) const;
    void load_bool_arr(std::string const& name, std::vector<int>  const& value) const;

private:

    int load(std::string const& file_name, int type);

    int programID {};

    size_t instance_max_count {1000};
};
