#include "model_util.h"

void model::Vao_Data::load_buffer_data(std::vector<float> const& vertices,
                                  std::vector<float> const& normals,
                                  std::vector<float> const& texture_coords,
                                  std::vector<int> const& indices)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    int vertices_attrib_array = 0;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(vertices_attrib_array, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    int normal_attrib_array = 1;
    glGenBuffers(1, &nb);
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(normal_attrib_array, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    int texture_attrib_array = 2;
    glGenBuffers(1, &tb);
    glBindBuffer(GL_ARRAY_BUFFER, tb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texture_coords.size(), &texture_coords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(texture_attrib_array, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices[0], GL_STATIC_DRAW);
    indices_count = indices.size();
    
    glEnableVertexAttribArray(vertices_attrib_array);
    glEnableVertexAttribArray(normal_attrib_array);
    glEnableVertexAttribArray(texture_attrib_array);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void model::Vao_Data::load_buffer_data(Buffer_Data const& data)
{
    load_buffer_data(data.vertices, data.normals, data.texture_coords, data.indices);
}

void model::Vao_Data::reload_buffer_data(std::vector<float> const& vertices,
                                    std::vector<float> const& normals,
                                    std::vector<float> const& texture_coords,
                                    std::vector<int> const& indices)
{
    glBindVertexArray(vao);

    int vertices_attrib_array = 0;
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(vertices_attrib_array, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    int normal_attrib_array = 1;
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(normal_attrib_array, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    int texture_attrib_array = 2;
    glBindBuffer(GL_ARRAY_BUFFER, tb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texture_coords.size(), &texture_coords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(texture_attrib_array, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices[0], GL_STATIC_DRAW);
    indices_count = indices.size();
    
    glEnableVertexAttribArray(vertices_attrib_array);
    glEnableVertexAttribArray(normal_attrib_array);
    glEnableVertexAttribArray(texture_attrib_array);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void model::Vao_Data::reload_buffer_data(Buffer_Data const& data)
{
    reload_buffer_data(data.vertices, data.normals, data.texture_coords, data.indices);
}

std::map<std::string, unsigned> model::loaded_textures = {};

unsigned int model::load_texture(std::string file_name, bool flip_y)
{
    auto it {loaded_textures.find(file_name)};
    if ( it != loaded_textures.end())
    {
        return it->second;
    } else
    {
        unsigned texture_id {load_texture_from_file(file_name, flip_y)};
        loaded_textures[file_name] = texture_id;
        return texture_id;
    }
    
}

unsigned int model::load_texture_from_file(std::string file_name, bool flip_y)
{
    unsigned int tex_id {0};

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flip_y); // tell stb to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glGenTextures(1, &tex_id);
        glBindTexture(GL_TEXTURE_2D, tex_id);

        if (nrChannels == 3)
        {
            data = stbi_load(file_name.c_str(), &width, &height, &nrChannels, STBI_rgb);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        //TODO: byt till att kasta ett undantag.
        std::cout << "Failed to load texture: " << file_name <<  std::endl;
    }
    stbi_image_free(data);

    return tex_id;
}



model::Vao_Data model::load_model_from_file(std::string const& file_name)
{
    objl::Loader obj_loader {};
    
    if (obj_loader.LoadFile("res/objects/" + file_name + "/" + file_name + ".obj"))
    {
        std::vector<float> vertices {};
        std::vector<float> normals {};
        std::vector<int> indices {};
        std::vector<float> texture_coords {};

        for (auto it = obj_loader.LoadedVertices.begin(); it != obj_loader.LoadedVertices.end(); it++)
        {
            vertices.push_back(it->Position.X);
            vertices.push_back(it->Position.Y);
            vertices.push_back(it->Position.Z);
            normals.push_back(it->Normal.X);
            normals.push_back(it->Normal.Y);
            normals.push_back(it->Normal.Z);
            texture_coords.push_back(it->TextureCoordinate.X);
            texture_coords.push_back(it->TextureCoordinate.Y);
        }

        for (auto it = obj_loader.LoadedIndices.begin(); it != obj_loader.LoadedIndices.end(); it++)
        {
            indices.push_back(*it);
        }

        objl::Material mat = obj_loader.LoadedMaterials[0];

        bool use_specularity_map {false};
        unsigned int specularity_map {};
        unsigned int kd_texture {load_texture("res/objects/" + file_name + "/" + mat.map_Kd)};
        if (mat.map_Ks != "")
        {
            specularity_map = load_texture("res/objects/" + file_name + "/" + mat.map_Ks);
            use_specularity_map = true;
        }
        Material material {kd_texture, specularity_map, use_specularity_map, mat.Ka, mat.Kd, mat.Ks, mat.Ni};

        Vao_Data model_data {};
        model_data.material = material;
        model_data.load_buffer_data(vertices, normals, texture_coords, indices);
        return model_data;

    } else
    {
        std::cout << "filename: " << file_name << std::endl;
        throw std::runtime_error("Cound not find model: " + file_name);
    }
}

model::Vao_Data model::load_obj_file(std::string const& file_path)
{

    objl::Loader obj_loader {};
    
    if (obj_loader.LoadFile(file_path))
    {
        std::vector<float> vertices {};
        std::vector<float> normals {};
        std::vector<int> indices {};
        std::vector<float> texture_coords {};

        for (auto it = obj_loader.LoadedVertices.begin(); it != obj_loader.LoadedVertices.end(); it++)
        {
            vertices.push_back(it->Position.X);
            vertices.push_back(it->Position.Y);
            vertices.push_back(it->Position.Z);
            normals.push_back(it->Normal.X);
            normals.push_back(it->Normal.Y);
            normals.push_back(it->Normal.Z);
            texture_coords.push_back(it->TextureCoordinate.X);
            texture_coords.push_back(it->TextureCoordinate.Y);
        }

        for (auto it = obj_loader.LoadedIndices.begin(); it != obj_loader.LoadedIndices.end(); it++)
        {
            indices.push_back(*it);
        }

        Vao_Data model_data {};
        model_data.load_buffer_data(vertices, normals, texture_coords, indices);
        return model_data;

    } else
    {
        throw std::runtime_error("Cound not find obj-file: " + file_path);
    }
}

model::Vao_Data model::get_billboard(std::string const& texture)
{

    std::vector<float> vertices {
        -0.5, -0.5, 0,
        -0.5,  0.5, 0,
         0.5, -0.5, 0,
         0.5,  0.5, 0,
    };
    std::vector<float> normals {
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
    };
    std::vector<float> tex_coords {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    };
    std::vector<int> indices {0,2,1,2,3,1};

    Vao_Data data {};
    data.load_buffer_data(vertices, normals, tex_coords, indices);
    data.material.texture_id = load_texture(texture);
    return data;
}
