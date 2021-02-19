#pragma once

#include "GL/gl.h"

#include "Vector.h"
#include "model_util.h"
//#include "Shader.h"
#include "Camera.h"

class Pos_Light
{
public:
    Pos_Light(vec3 const& pos, vec3 const& color)
        : pos {pos}, color {color}, attenuation_constant{1.0}, attenuation_linear {0.045}, attenuation_quadratic {0.0075} {}

    vec3 get_position() const { return pos; }
    vec3 get_color() const { return color; }
    float get_attenuation_constant() const { return attenuation_constant; }
    float get_attenuation_linear() const { return attenuation_linear; }
    float get_attenuation_quadratic() const { return attenuation_quadratic; }

    void set_position(vec3 const& new_pos) { pos = new_pos; }

private:
    vec3 pos;
    vec3 color {1, 1, 1};
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;
};

class Dir_Light
{
public:
    Dir_Light(vec3 const& dir, vec3 const& color)
        : dir {dir}, color {color} {}

    vec3 get_direction() const {return dir; }
    vec3 get_color() const { return color; }

protected:
    vec3 dir;
    vec3 color {1, 1, 1};
};

class Sun : public Dir_Light
{
public:

    Sun();

    void update(float delta_time);

    vec3 get_position() const { return pos; }
    mat4 get_rotation() const { return rot; }
    model::Vao_Data const& get_vao() const { return billboard; }

private:

    model::Vao_Data billboard {model::get_billboard("res/images/sun.png")};

    vec3 pos {40, 5, 0};
    mat4 rot {};
};

class Light_Container
{
public:
    void add_pos_light(vec3 const& position,  vec3 const& color);
    void add_dir_light(vec3 const& direction, vec3 const& color);

    void update(float delta_time);

    vec3 get_sun_position() const { return sun.get_position(); }
    vec2 get_sun_screen_position(Camera const * camera) const;
    Sun const& get_sun() const { return sun; }

    int get_number_of_lights() const {return pos_lights.size() + dir_lights.size() + 1; }
    
    std::vector<vec3> get_pos_dir_data() const;
    std::vector<vec3> get_color_data() const;
    std::vector<vec3> get_attenuation_data() const;
    std::vector<int> get_light_type_data() const;

    std::vector<Pos_Light> get_pos_lights() const { return pos_lights; }
    std::vector<Dir_Light> get_dir_lights() const { return dir_lights; }

private:
    std::vector<Pos_Light> pos_lights {};
    std::vector<Dir_Light> dir_lights {};
    Sun sun {};

    model::Vao_Data pos_light_vao_data {model::load_obj_file("res/objects/light.obj")};
};