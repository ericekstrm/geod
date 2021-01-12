#include "Skybox.h"

#include <algorithm>
#include <iostream>
#include "stb_image.h"

Skybox::Skybox()
{
    load_cube_map(bluecloud);
    load_buffer_data();
}

Skybox::~Skybox()
{
}

void Skybox::render() const
{
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(model_data.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
    glDrawElements(GL_TRIANGLES, model_data.indices_count, GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
}

void Skybox::load_cube_map(std::vector<std::string> texture_files)
{
    glGenTextures(1, &cubemap_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);

    for (unsigned i = 0; i < texture_files.size(); i++)
    {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false); // tell stb to not flip loaded texture's on the y-axis.
        unsigned char *data = stbi_load(texture_files[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else
        {
            std::cout << "Failed to load texture:" << texture_files[i] << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::load_buffer_data()
{
    glGenVertexArrays(1, &model_data.vao);
    glBindVertexArray(model_data.vao);

    int vertices_attrib_array = 0;
    glGenBuffers(1, &model_data.vb);
    glBindBuffer(GL_ARRAY_BUFFER, model_data.vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(vertices_attrib_array, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    std::vector<int> i(vertices.size());
    std::generate(i.begin(), i.end(), [i = 0]() mutable {
        return i++;
    });

    glGenBuffers(1, &model_data.ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_data.ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * i.size(), &i[0], GL_STATIC_DRAW);
    model_data.indices_count = i.size();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
