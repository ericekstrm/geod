#pragma once

#include <string>
#include "Model.h"

class Skybox : public Model
{
public:
    Skybox();
    ~Skybox();

    virtual void render() const;

private:
    unsigned int texture_id;
    void load_cube_map(std::vector<std::string> texture_files);
    void load_buffer_data();

    unsigned int cubemap_id {};
    void load_cubemap_VBO(std::vector<float> const & texture_coords);

    //====| data |====

    float SIZE {5};
    std::vector<float> vertices {
        -SIZE, SIZE, -SIZE,
        -SIZE, -SIZE, -SIZE,
        SIZE, -SIZE, -SIZE,
        SIZE, -SIZE, -SIZE,
        SIZE, SIZE, -SIZE,
        -SIZE, SIZE, -SIZE,
        -SIZE, -SIZE, SIZE,
        -SIZE, -SIZE, -SIZE,
        -SIZE, SIZE, -SIZE,
        -SIZE, SIZE, -SIZE,
        -SIZE, SIZE, SIZE,
        -SIZE, -SIZE, SIZE,
        SIZE, -SIZE, -SIZE,
        SIZE, -SIZE, SIZE,
        SIZE, SIZE, SIZE,
        SIZE, SIZE, SIZE,
        SIZE, SIZE, -SIZE,
        SIZE, -SIZE, -SIZE,
        -SIZE, -SIZE, SIZE,
        -SIZE, SIZE, SIZE,
        SIZE, SIZE, SIZE,
        SIZE, SIZE, SIZE,
        SIZE, -SIZE, SIZE,
        -SIZE, -SIZE, SIZE,
        -SIZE, SIZE, -SIZE,
        SIZE, SIZE, -SIZE,
        SIZE, SIZE, SIZE,
        SIZE, SIZE, SIZE,
        -SIZE, SIZE, SIZE,
        -SIZE, SIZE, -SIZE,
        -SIZE, -SIZE, -SIZE,
        -SIZE, -SIZE, SIZE,
        SIZE, -SIZE, -SIZE,
        SIZE, -SIZE, -SIZE,
        -SIZE, -SIZE, SIZE,
        SIZE, -SIZE, SIZE
    };

    std::vector<std::string> grimmnight {
        "res/skybox/grimmnight_rt.tga",
        "res/skybox/grimmnight_lf.tga",
        "res/skybox/grimmnight_up.tga",
        "res/skybox/grimmnight_dn.tga",
        "res/skybox/grimmnight_bk.tga",
        "res/skybox/grimmnight_ft.tga"
    };

    std::vector<std::string> violentdays {
        "res/skybox/violentdays_rt.tga",
        "res/skybox/violentdays_lf.tga",
        "res/skybox/violentdays_up.tga",
        "res/skybox/violentdays_dn.tga",
        "res/skybox/violentdays_bk.tga",
        "res/skybox/violentdays_ft.tga"
    };

    std::vector<std::string> bluecloud {
        "res/skybox/bluecloud_rt.jpg",
        "res/skybox/bluecloud_lf.jpg",
        "res/skybox/bluecloud_up.jpg",
        "res/skybox/bluecloud_dn.jpg",
        "res/skybox/bluecloud_bk.jpg",
        "res/skybox/bluecloud_ft.jpg"
    };
};
