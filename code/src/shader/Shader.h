#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "Matrix.h"
#include "model_util.h"

class Shader
{
public:
    Shader();
    Shader(std::string const& vertex_file, std::string const& fragment_file);
    virtual ~Shader();

    void start() const;
    void stop() const;

    int get_programID() const;

    void load_projection_matrix() const;
    void load_camera_matrix(Matrix4 const& mat) const;

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
};

// =======================
// ===| Skybox Shader |===
// =======================

class Skybox_Shader : public Shader
{
public:
    Skybox_Shader();
};

// =====================
// ===| Text Shader |===
// =====================

class Text_Shader : public Shader
{
public:
    Text_Shader();
    ~Text_Shader();

    void load_font_color(vec3 const& color) const;
    void load_text_pos_matrix(Matrix4 const& mat) const;
    void load_char_pos_matrix(Matrix4 const& mat) const;
};

// ========================
// ===| Image2D Shader |===
// ========================

class Image2D_Shader : public Shader
{
public:
    Image2D_Shader();
    ~Image2D_Shader();

    void load_pos_matrix(vec2 const& pos) const;
};

class Color_Point_Shader : public Shader
{
public:
    Color_Point_Shader();
    void load_position(vec3 const& pos) const;
    void load_color(vec3 const& color) const;
};

// ==========================
// ===| Billboard Shader |===
// ==========================

class Billboard_Shader : public Shader
{
public:
    Billboard_Shader();
    Billboard_Shader(std::string const& vertex_file, std::string const& fragment_file);
    ~Billboard_Shader();

    void load_model_matrix(Matrix4 const& mat) const;
    void load_color(vec3 const& color) const;
};