#include "Renderer.h"

void Renderer::render(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, std::initializer_list<Model const*> models) const
{
    shader.start();
    shader.load_projection_matrix();
    shader.load_camera_matrix(camera->get_camera_matrix());
    shader.load_camera_position(camera->get_position());
    shader.load_lights(lights);
    shader.load_light_space_matrix(shadowmap.get_light_position());

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, shadowmap.get_texture_id());

    for (auto it = models.begin(); it != models.end(); it++)
    {
        glBindVertexArray((*it)->get_model_data().vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (*it)->get_model_data().material.texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        shader.load_model_matrix((*it)->get_model_matrix());
        shader.load_material_properties((*it)->get_material());

        glDrawElements(GL_TRIANGLES, (*it)->get_model_data().indices_count, GL_UNSIGNED_INT, 0);
    }

    shader.stop();
}

void Renderer::render_to_shadowmap(Shadowmap shadowmap, std::initializer_list<Model const*> models) const
{
    shadowmap.activate();
    glCullFace(GL_FRONT);
    shadowmap_shader.start();
    shadowmap_shader.load_light_space_matrix(shadowmap.get_light_position());

    for (auto it = models.begin(); it != models.end(); it++)
    {
        glBindVertexArray((*it)->get_model_data().vao);

        shadowmap_shader.load_model_matrix((*it)->get_model_matrix());

        glDrawElements(GL_TRIANGLES, (*it)->get_model_data().indices_count, GL_UNSIGNED_INT, 0);
    }
    shadowmap_shader.stop();
    
    glCullFace(GL_BACK);
    shadowmap.deactivate();
}

void Renderer::render_godray(Framebuffer const& fbo, Light_Container const& lights, Camera const* camera, std::initializer_list<Model const*> models) const
{
    fbo.bind();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lights.render_sun(camera);

    god_ray_shader.start();
    god_ray_shader.load_projection_matrix();
    god_ray_shader.load_camera_matrix(camera->get_camera_matrix());

    for (auto it = models.begin(); it != models.end(); it++)
    {
        glBindVertexArray((*it)->get_model_data().vao);

        god_ray_shader.load_model_matrix((*it)->get_model_matrix());

        glDrawElements(GL_TRIANGLES, (*it)->get_model_data().indices_count, GL_UNSIGNED_INT, 0);
    }

    god_ray_shader.stop();

    fbo.unbind();
}


void Renderer::render_skybox(Skybox const& skybox, Camera const* camera) const
{
    skybox_shader.start();
    skybox_shader.load_projection_matrix();
    skybox_shader.load_camera_matrix(camera->get_camera_matrix().remove_translation());

    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(skybox.get_model_data().vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.get_cubemap_id());
    glDrawElements(GL_TRIANGLES, skybox.get_model_data().indices_count, GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
    skybox_shader.stop();
}

//TODO: instance render 