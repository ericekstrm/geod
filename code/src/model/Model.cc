#include "Model.h"

#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <string>
#include <iostream>

Model::Model(std::string const& file_name, vec3 const & position)
    : model_data {model::load_model_from_file(file_name)}, position {position}
{}

Matrix4 const Model::get_model_matrix() const
{
    Matrix4 t {translation_matrix(position.x, position.y, position.z)};
    Matrix4 s {scale_matrix(scale.x, scale.y, scale.z)};
    Matrix4 r { rotation_matrix(rotation.x, 1, 0, 0) * 
                rotation_matrix(rotation.y, 0, 1, 0) *
                rotation_matrix(rotation.z, 0, 0, 1) };

    return (t * r * s);
}
