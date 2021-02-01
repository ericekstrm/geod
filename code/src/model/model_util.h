#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "GL/gl.h"

#include "stb_image.h"
#include "OBJ_Loader.h"
#include "Vector.h"

namespace model
{

    struct Material
    {
        unsigned int texture_id {};
        unsigned int specularity_map_id {};
        bool use_specularity_map {};
        vec3 ka {};
        vec3 kd {};
        vec3 ks {};
        float a {};
    };

    struct Buffer_Data
    {
        std::vector<float> vertices {};
        std::vector<float> normals {};
        std::vector<float> texture_coords {};
        std::vector<int> indices {};
    };

    struct Vao_Data
    {
        void load_buffer_data(std::vector<float> const&, std::vector<float> const&, std::vector<float> const&, std::vector<int> const&);
        void load_buffer_data(Buffer_Data const& data);
        void reload_buffer_data(std::vector<float> const&, std::vector<float> const&, std::vector<float> const&, std::vector<int> const&);
        void reload_buffer_data(Buffer_Data const& data);
        unsigned int vao {};
        unsigned int vb {}, nb {}, tb {}, ib {};
        unsigned int indices_count {};
        Material material {};

        Vao_Data(Vao_Data const&) = delete;
        Vao_Data(Vao_Data &&) = default;
    };

    extern std::map<std::string, unsigned> loaded_textures;
    unsigned int load_texture(std::string file_name, bool flip_y = true);
    unsigned int load_texture_from_file(std::string file_name, bool flip_y = true);

    /**
     *  Loads a model from file into a vao by the name of the folder in "res/objects"
     */
    Vao_Data load_model_from_file(std::string const& file_name);

    /**
     *  Loads an obj-file into a vao. 
     * 
     *  file_path is given as a relative path to the file. 
     */
    Vao_Data load_obj_file(std::string const& file_path);

    
    Vao_Data get_billboard(std::string const& texture);
}
