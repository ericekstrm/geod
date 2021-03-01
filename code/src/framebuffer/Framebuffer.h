#pragma once

class Framebuffer
{
public:
    Framebuffer(int width, int height);
    Framebuffer();

    void clear() const;
    void bind() const;
    void unbind() const;

    unsigned get_fbo_id() const { return fbo; }
    unsigned get_texture_id() const { return texture_id; }
    unsigned get_depth_id() const { return depth_id; }

protected:

    void create_buffer();
    void create_texture_attachment(int width, int height);
    void create_depth_buffer_attachment(int width, int height);
    void create_depth_texture_attachment(int width, int height);

    int width;
    int height;

    unsigned fbo;
    unsigned texture_id;
    unsigned depth_id;
};