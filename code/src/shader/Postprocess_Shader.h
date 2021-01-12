#pragma once

#include "Shader.h"

class Postprocess_Shader : public Shader
{
public:
    Postprocess_Shader();
    ~Postprocess_Shader();

    void load_pos_matrix(vec2 const& pos) const;
    void load_sun_pos(vec2 const& sun_pos) const;
};