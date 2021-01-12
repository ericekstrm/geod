#pragma once

#if (defined (_WIN32) || defined (_WIN64))
    #include "glad/glad.h"
#else
    #include "GL/gl.h"
#endif

#include "Vector.h"
#include "Shader.h"

class Image
{
public:
    Image(vec2 const& position, vec2 size, std::string image_file);
    Image(vec2 const& position, vec2 size, unsigned texture_id);

    virtual void render() const;

    void set_texture_id(unsigned new_id) { texture_id = new_id; }

protected:
    vec2 position;
    vec2 size;

    void create_mesh();

    unsigned vao;
    unsigned vb;
    unsigned tb;
    unsigned ib;
    int indices_count;

private:
    unsigned texture_id;

    Image2D_Shader shader {};
};
