#pragma once

#include "GL/gl.h"

#include "Vector.h"
#include "Matrix.h"
#include "Model_Shader.h"
#include "model_util.h"

#include <vector>
#include <string>
#include <map>

class Model
{
public:
    Model();
    Model(std::string const& file_name, vec3 const& position = {0, 0, 0});
    Model(std::vector<float> vertices, 
        std::vector<float> normals, 
        std::vector<float> texture_coords, 
        std::vector<int> indices,
        objl::Material material);
    ~Model();

    void update(float delta_time);

    void set_position(vec3 const& new_pos) { position = new_pos; }
    void set_scale(vec3 const& new_scale) { scale = new_scale; }
    void set_rotation(vec3 const& new_rot) { rotation = new_rot; }

    Matrix4 const get_model_matrix() const;
    model::Material get_material() const { return model_data.material; }
    void set_material(model::Material new_mat) { model_data.material = new_mat; }
    model::Vao_Data const& get_model_data() const { return model_data; }

protected:

    model::Vao_Data model_data {};

    vec3 position {};
    vec3 scale {1, 1, 1};
    vec3 rotation {};
};
