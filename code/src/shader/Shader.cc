#include "Shader.h"

#ifdef WIN32
    #include "glad/glad.h"
#else
    #include "GL/gl.h"
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <iterator>

#include "Vector.h"

#include "settings.h"

Shader::Shader()
    : Shader("pass.vert", "pass.frag")
{
}

Shader::Shader(std::string const& vertex_file, std::string const& fragment_file)
{
    int vertexID = load(vertex_file, GL_VERTEX_SHADER);
    int fragmentID = load(fragment_file, GL_FRAGMENT_SHADER);
    programID = glCreateProgram();
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    glLinkProgram(programID);

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    glUseProgram(programID);
}

Shader::~Shader()
{
    glDeleteProgram(programID);
}

void Shader::start() const
{
    glUseProgram(programID);
}

void Shader::stop() const
{
    glUseProgram(0);
}

int Shader::get_uniform_location(std::string const& uniform_name) const
{
    const GLchar* c = uniform_name.c_str();
    return glGetUniformLocation(programID, c);
}

void Shader::load_int(std::string const& name, int value) const
{
    glUniform1i(get_uniform_location(name), value);
}

void Shader::load_float(std::string const& name, float value) const
{
    glUniform1f(get_uniform_location(name), value);
}

void Shader::load_bool(std::string const& name, bool b) const
{
    glUniform1i(get_uniform_location(name), b ? 1 : 0);
}

void Shader::load_mat4(std::string const& name, mat4 const& value) const
{
    mat4 tmp {value.transpose()};
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &tmp.m[0][0]);
}

void Shader::load_vec2(std::string const& name, vec2 const& value) const
{
    glUniform2f(get_uniform_location(name), value.x, value.y);
}

void Shader::load_vec3(std::string const& name, vec3 const& value) const
{
    glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
}

void Shader::load_vec4(std::string const& name, vec4 const& value) const
{
    glUniform4f(get_uniform_location(name), value[0], value[1], value[2], value[3]);
}

void Shader::load_bool_arr(std::string const& name, std::vector<int> const& value) const
{
    glUniform1iv(get_uniform_location(name), value.size(), reinterpret_cast<const int *>(value.data()));
}

void Shader::load_vec3_arr(std::string const& name, std::vector<vec3> const& value) const
{
    // undrar om detta fungerar som man väntar sig, inte helt säkert eftersom vi har min egen vec3.
    // Det fungerar med Ingemars vec3 men det är ju verkligen inte en garanti att det fungerar senare, hehe.
    glUniform3fv(get_uniform_location(name), value.size(), reinterpret_cast<const float *>(value.data()));
}

int Shader::get_programID() const
{
    return programID;
}

void Shader::load_projection_matrix() const
{
    load_mat4("projection_matrix", projection);
}

void Shader::load_camera_matrix(Matrix4 const & mat) const
{
    load_mat4("world_matrix", mat);
}

int Shader::load(std::string const & file_name, int type)
{
    std::ifstream shader_source {"res/shader_files/" + file_name};
    if (!shader_source.is_open())
    {
        std::cerr << "Shader file not found: " << file_name << std::endl;
        return 0;
    }

    // now read in the data
    std::string str_source = std::string((std::istreambuf_iterator<char>(shader_source)), std::istreambuf_iterator<char>());
    shader_source.close();
    str_source += "\0";

    int shaderID = glCreateShader(type);

    const char* data = str_source.c_str();
    glShaderSource(shaderID, 1, &data, NULL);
    glCompileShader(shaderID);

    int is_compiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

        std::copy(errorLog.begin(), errorLog.end(), std::ostream_iterator<GLchar> {std::cerr});
    }
    return shaderID;
}

//=======================
//===| Skybox Shader |===
//=======================

Skybox_Shader::Skybox_Shader()
    : Shader {"skybox.vert", "skybox.frag"}
{
}

// =====================
// ===| Text Shader |===
// =====================

Text_Shader::Text_Shader()
    : Shader{"text.vert", "text.frag"}
{
}

Text_Shader::~Text_Shader()
{
}

void Text_Shader::load_font_color(vec3 const& color) const
{
    load_vec3("font_color", color);
}

void Text_Shader::load_text_pos_matrix(Matrix4 const& mat) const
{
    load_mat4("text_pos_matrix", mat);
}

void Text_Shader::load_char_pos_matrix(Matrix4 const& mat) const
{
    load_mat4("char_pos_matrix", mat);
}

// ========================
// ===| Image2D Shader |===
// ========================

Image2D_Shader::Image2D_Shader()
    : Shader{"image2d.vert", "image2d.frag"}
{
}

Image2D_Shader::~Image2D_Shader()
{
}

void Image2D_Shader::load_pos_matrix(vec2 const& pos) const
{
    load_mat4("pos_matrix", translation_matrix(pos.x, pos.y, 0));
}

// ============================
// ===| Color Point Shader |===
// ============================

Color_Point_Shader::Color_Point_Shader()
    : Shader {"color_point.vert", "color_point.frag"}
{
}

void Color_Point_Shader::load_position(vec3 const& pos) const
{
    load_vec3("position", pos);
}

void Color_Point_Shader::load_color(vec3 const& color) const
{
    load_vec3("color", color);
}

// ==========================
// ===| Billboard Shader |===
// ==========================

Billboard_Shader::Billboard_Shader(std::string const& vertex_file, std::string const& fragment_file)
    : Shader(vertex_file, fragment_file)
{

}

Billboard_Shader::Billboard_Shader()
    : Shader {"billboard.vert", "billboard.frag"}
{
}

Billboard_Shader::~Billboard_Shader()
{    

}

void Billboard_Shader::load_model_matrix(Matrix4 const& mat) const
{
    load_mat4("model_matrix", mat);
}

void Billboard_Shader::load_color(vec3 const& color) const
{
    load_vec3("color", color);
}