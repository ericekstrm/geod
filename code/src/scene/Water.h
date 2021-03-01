#pragma once

#include "Framebuffer.h"
#include "model_util.h"

class Water_Handler
{
public:

    Water_Handler();

    void bind_reflection_framebuffer() const;
    void bind_refraction_framebuffer() const;
    void unbind_framebuffer() const;

    Framebuffer get_reflection_framebuffer() const;
    Framebuffer get_refraction_framebuffer() const;

    model::Vao_Data const& get_water_tile() const { return water_tile; }

    float get_height() const { return height; }

private:

    float height {-0.15};

    int reflection_width  {1280};
    int reflection_heigth {720};
    int refraction_width  {1280};
    int refraction_heigth {720};

    Framebuffer reflection_framebuffer;
    Framebuffer refraction_framebuffer;

    model::Vao_Data water_tile;

};