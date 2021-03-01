#pragma once

#include <string>
#include "Model.h"

class Skybox : public Model
{
public:
    Skybox();
    ~Skybox();

    unsigned int get_cubemap_id() const { return cubemap_id; }

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

    std::vector<std::string> yellowcloud {
        "res/skybox/yellowcloud_rt.jpg",
        "res/skybox/yellowcloud_lf.jpg",
        "res/skybox/yellowcloud_up.jpg",
        "res/skybox/yellowcloud_dn.jpg",
        "res/skybox/yellowcloud_bk.jpg",
        "res/skybox/yellowcloud_ft.jpg"
    };

    std::vector<std::string> stormydays {
        "res/skybox/stormydays_rt.tga",
        "res/skybox/stormydays_lf.tga",
        "res/skybox/stormydays_up.tga",
        "res/skybox/stormydays_dn.tga",
        "res/skybox/stormydays_bk.tga",
        "res/skybox/stormydays_ft.tga"
    };
};
