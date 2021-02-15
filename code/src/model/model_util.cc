#include "model_util.h"

void model::Vao_Data::load_buffer_data(std::vector<float> const& vertices,
                                  std::vector<float> const& normals,
                                  std::vector<float> const& texture_coords,
                                  std::vector<float> const& tangents,
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

    int tangent_attrib_array = 3;
    glGenBuffers(1, &tab);
    glBindBuffer(GL_ARRAY_BUFFER, tab);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size(), &tangents[0], GL_STATIC_DRAW);
    glVertexAttribPointer(tangent_attrib_array, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices[0], GL_STATIC_DRAW);
    indices_count = indices.size();
    
    glEnableVertexAttribArray(vertices_attrib_array);
    glEnableVertexAttribArray(normal_attrib_array);
    glEnableVertexAttribArray(texture_attrib_array);
    glEnableVertexAttribArray(tangent_attrib_array);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void model::Vao_Data::load_buffer_data(Buffer_Data const& data)
{
    load_buffer_data(data.vertices, data.normals, data.texture_coords, data.tangents, data.indices);
}

void model::Vao_Data::reload_buffer_data(std::vector<float> const& vertices,
                                    std::vector<float> const& normals,
                                    std::vector<float> const& texture_coords,
                                    std::vector<float> const& tangents,
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

    int tangent_attrib_array = 3;
    glBindBuffer(GL_ARRAY_BUFFER, tab);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size(), &tangents[0], GL_STATIC_DRAW);
    glVertexAttribPointer(tangent_attrib_array, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices[0], GL_STATIC_DRAW);
    indices_count = indices.size();
    
    glEnableVertexAttribArray(vertices_attrib_array);
    glEnableVertexAttribArray(normal_attrib_array);
    glEnableVertexAttribArray(texture_attrib_array);
    glEnableVertexAttribArray(tangent_attrib_array);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void model::Vao_Data::reload_buffer_data(Buffer_Data const& data)
{
    reload_buffer_data(data.vertices, data.normals, data.texture_coords, data.tangents, data.indices);
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
        std::vector<float> tangents {};
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

        // Add tangent data
        tangents.resize(normals.size(), 0);
        for (int i = 0; i < indices.size(); i+=3)
        {
            vec3 p1 {vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]};
            vec3 p2 {vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]};
            vec3 p3 {vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]};
            vec2 t1 {texture_coords[indices[i] * 2], texture_coords[indices[i] * 2 + 1]};
            vec2 t2 {texture_coords[indices[i + 1] * 2], texture_coords[indices[i + 1] * 2 + 1]};
            vec2 t3 {texture_coords[indices[i + 2] * 2], texture_coords[indices[i + 2] * 2 + 1]};

            vec3 p12 {p1 - p2};
            vec3 p23 {p2 - p3};
            vec3 p31 {p3 - p1};
            vec2 t12 {t1 - t2};
            vec2 t23 {t2 - t3};
            vec2 t31 {t3 - t1};

            /*vec3 T1 {(p12 - p31 * t12.y / t31.y) / (t12.x - t12.y * t31.x / t31.y)};
            vec3 T2 {(p12 - p23 * t12.y / t23.y) / (t12.x - t12.y * t23.x / t23.y)};
            vec3 T3 {(p23 - p31 * t23.y / t31.y) / (t23.x - t23.y * t31.x / t31.y)};

            std::cout << "------" << std::endl;
            std::cout << "t1: " << t1 << std::endl;
            std::cout << "t2: " << t2 << std::endl;
            std::cout << "t3: " << t3 << std::endl;
            std::cout << "p12: " << p12 << std::endl;
            std::cout << "p23: " << p23 << std::endl;
            std::cout << "t12: " << t12 << std::endl;
            std::cout << "t23: " << t23 << std::endl;

            tangents[indices[i] * 3]     = T1.x;
            tangents[indices[i] * 3 + 1] = T1.y;
            tangents[indices[i] * 3 + 1] = T1.z;
            tangents[indices[i + 1] * 3]     = T2.x;
            tangents[indices[i + 1] * 3 + 1] = T2.y;
            tangents[indices[i + 1] * 3 + 1] = T2.z;
            tangents[indices[i + 2] * 3]     = T3.x;
            tangents[indices[i + 2] * 3 + 1] = T3.y;
            tangents[indices[i + 2] * 3 + 1] = T3.z;*/

            //from opengl-tutorial.org
            float r {1.0f / (t12.x * t23.y - t12.y * t23.x)};
            vec3 T = (p12 * t23.y - p23 * t12.y) * r;
            T.normalize();

            //std::cout << "tangent: " << T << std::endl;

            tangents[indices[i] * 3]     = T.x;
            tangents[indices[i] * 3 + 1] = T.y;
            tangents[indices[i] * 3 + 1] = T.z;
            tangents[indices[i + 1] * 3]     = T.x;
            tangents[indices[i + 1] * 3 + 1] = T.y;
            tangents[indices[i + 1] * 3 + 1] = T.z;
            tangents[indices[i + 2] * 3]     = T.x;
            tangents[indices[i + 2] * 3 + 1] = T.y;
            tangents[indices[i + 2] * 3 + 1] = T.z;
        }


        objl::Material mat = obj_loader.LoadedMaterials[0];

        unsigned int kd_texture {load_texture("res/objects/" + file_name + "/" + mat.map_Kd)};
        Material material {};
        material.texture_id = kd_texture;
        material.ka = mat.Ka;
        material.kd = mat.Kd;
        material.ks = mat.Ks;
        material.a = mat.Ni;

        Vao_Data model_data {};
        model_data.material = material;
        model_data.load_buffer_data(vertices, normals, texture_coords, tangents, indices);
        return model_data;

    } else
    {
        std::cout << "filename: " << file_name << std::endl;
        throw std::runtime_error("Could not find model: " + file_name);
    }
}

model::Vao_Data model::load_obj_file(std::string const& file_path)
{

    objl::Loader obj_loader {};
    
    if (obj_loader.LoadFile(file_path))
    {
        std::vector<float> vertices {};
        std::vector<float> normals {};
        std::vector<float> texture_coords {};
        std::vector<float> tangents {};
        std::vector<int> indices {};

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

        // Add tangent data
        for (int i = 0; i < indices.size(); i+=3)
        {
            vec3 p1 {vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]};
            vec3 p2 {vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]};
            vec3 p3 {vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]};
            vec2 t1 {texture_coords[indices[i] * 2], texture_coords[indices[i] * 2 + 1]};
            vec2 t2 {texture_coords[indices[i + 1] * 2], texture_coords[indices[i + 1] * 2 + 1]};
            vec2 t3 {texture_coords[indices[i + 2] * 2], texture_coords[indices[i + 2] * 2 + 1]};

            vec3 p12 {p1 - p2};
            vec3 p23 {p2 - p3};
            vec3 p31 {p3 - p1};
            vec2 t12 {t1 - t2};
            vec2 t23 {t2 - t3};
            vec2 t31 {t3 - t1};

            vec3 T1 {(p12 - p31 * t12.y / t31.y) / (t12.x - t12.y * t31.x / t31.y)};
            vec3 T2 {(p12 - p23 * t12.y / t23.y) / (t12.x - t12.y * t23.x / t23.y)};
            vec3 T3 {(p23 - p31 * t23.y / t31.y) / (t23.x - t23.y * t31.x / t31.y)};

            tangents.push_back(T1.x);
            tangents.push_back(T1.y);
            tangents.push_back(T1.z);
            tangents.push_back(T2.x);
            tangents.push_back(T2.y);
            tangents.push_back(T2.z);
            tangents.push_back(T3.x);
            tangents.push_back(T3.y);
            tangents.push_back(T3.z);
        }

        Vao_Data model_data {};
        model_data.load_buffer_data(vertices, normals, texture_coords, tangents, indices);
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
    std::vector<float> tangents {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
    };
    std::vector<float> tex_coords {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    };
    std::vector<int> indices {0,2,1,2,3,1};

    Vao_Data data {};
    data.load_buffer_data(vertices, normals, tex_coords, tangents, indices);
    data.material.texture_id = load_texture(texture);
    return data;
}
