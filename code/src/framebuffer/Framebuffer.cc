#include "Framebuffer.h"

#include "Settings.h"

#include "GL/gl.h"

Framebuffer::Framebuffer()
    : Framebuffer {Settings::get_window_width(), Settings::get_window_height()}
{}

Framebuffer::Framebuffer(int w, int h)
    : width {w}, height {h}
{
    create_buffer();
    create_texture_attachment(width, height);
    create_depth_texture_attachment(width, height);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! "
                  << "Code: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl; 
    }
    unbind();
}

void Framebuffer::clear() const 
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() const
{
    glViewport(0, 0, width, height);
    glBindTexture(GL_TEXTURE, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Settings::get_window_width(), Settings::get_window_height());
}

void Framebuffer::create_buffer()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void Framebuffer::create_texture_attachment(int width, int height)
{
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
}

void Framebuffer::create_depth_buffer_attachment(int width, int height)
{
    glGenRenderbuffers(1, &depth_id);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_id);
}

void Framebuffer::create_depth_texture_attachment(int width, int height)
{
    glGenTextures(1, &depth_id);
    glBindTexture(GL_TEXTURE_2D, depth_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_id, 0);
}