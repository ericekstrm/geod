#pragma once

class Framebuffer
{
public:
    Framebuffer();

    void bind() const;
    void unbind() const;

    unsigned get_texture_id() const { return texture_id; }

private:

    void create_buffer();
    void create_texture_attachment(int width, int height);
    void create_depth_attachment(int width, int height);

    unsigned fbo;
    unsigned texture_id;
    unsigned depth_id;
};