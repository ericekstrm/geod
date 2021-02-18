#pragma once

#include "GL/gl.h"

#include "Vector.h"
#include "Shader.h"

class Image
{
public:
    Image(vec2 const& position, vec2 size, std::string image_file);
    Image(vec2 const& position, vec2 size, unsigned texture_id);

    virtual void render() const;

    void set_texture_id(unsigned new_id) { vao_data.material.texture_id = new_id; }

protected:
    vec2 position;
    vec2 size;

    model::Vao_Data vao_data;

private:

    Image2D_Shader shader {};
};
