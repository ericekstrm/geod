#include "Model.h"

#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <string>
#include <iostream>

Model::Model()
    : Model {"door", vec3{0, 0, 0}}
{
}

Model::Model(std::string const& file_name, vec3 const & position)
    : position {position}
{
    load_model(file_name);
}

Model::Model(std::vector<float> vertices, std::vector<float> normals, std::vector<float> texture_coords, std::vector<int> indices, objl::Material)
{
    model_data.load_buffer_data(vertices, normals, texture_coords, indices);
    //TODO: model_data.material = (objl::)material;
}

Model::Model(model::Vao_Data const& vao_data)
    : model_data {vao_data}
{}

Model::~Model()
{
}

void Model::update(float)
{
}

void Model::render(Model_Shader const * shader) const
{
    glBindVertexArray(model_data.vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model_data.material.texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    shader->load_model_matrix(get_model_matrix());
    shader->load_material_properties(this->get_material());

    glDrawElements(GL_TRIANGLES, model_data.indices_count, GL_UNSIGNED_INT, 0);
}

Matrix4 const Model::get_model_matrix() const
{
    Matrix4 t {translation_matrix(position.x, position.y, position.z)};
    Matrix4 s {scale_matrix(scale.x, scale.y, scale.z)};
    Matrix4 r {
        rotation_matrix(rotation.x, 1, 0, 0) * 
        rotation_matrix(rotation.y, 0, 1, 0) *
        rotation_matrix(rotation.z, 0, 0, 1)};

    return (t * r * s);
}

model::Material Model::get_material() const
{
    return model_data.material;
}

void Model::load_model(std::string const& file_name)
{
    model::Vao_Data current_model;

    auto it = models.find(file_name);
    if ( it != models.end())
    {
        current_model = it->second;
    } else
    {
        current_model = model::load_model_from_file(file_name);
        models[file_name] = current_model;
    }

    model_data = current_model;
}
