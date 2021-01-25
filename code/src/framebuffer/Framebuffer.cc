#include "Framebuffer.h"

#include "settings.h"

#include "GL/gl.h"

const int WIDTH {1024};
const int HEIGHT {1024};

Framebuffer::Framebuffer()
{
    create_buffer();
    create_texture_attachment(WIDTH, HEIGHT);
    create_depth_attachment(WIDTH, HEIGHT);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! "
                  << "Code: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl; 
    unbind();
}

void Framebuffer::bind() const
{
    glBindTexture(GL_TEXTURE, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, WIDTH, HEIGHT);
}

void Framebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_width, window_height);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
}

void Framebuffer::create_depth_attachment(int width, int height)
{
    glGenRenderbuffers(1, &depth_id);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_id);
}