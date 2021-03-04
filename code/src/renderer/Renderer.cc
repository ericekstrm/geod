#include "Renderer.h"

void Renderer::render(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, Scene const& scene, vec4 const& clipping_plane) const
{
    pbr_terrain_shader.start();
    pbr_terrain_shader.load_projection_matrix();
    pbr_terrain_shader.load_camera_matrix(camera->get_camera_matrix());
    pbr_terrain_shader.load_camera_position(camera->get_position());
    pbr_terrain_shader.load_lights(lights);
    pbr_terrain_shader.load_light_space_matrix(shadowmap.get_light_position());
    pbr_terrain_shader.load_clipping_plane(clipping_plane);

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, shadowmap.get_texture_id());

    auto & road_models {scene.get_road().get_lanes()};
    for (auto & lane : road_models)
    {

        model::Vao_Data const& model_data {lane.second->get_vao()};
        glBindVertexArray(model_data.vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model_data.material.map_albedo);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, model_data.material.map_normal);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, model_data.material.map_metal);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, model_data.material.map_rough);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, model_data.material.map_ao);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, model_data.material.map_opacity);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        pbr_terrain_shader.load_model_matrix(mat4{});

        glDrawElements(GL_TRIANGLES, model_data.indices_count, GL_UNSIGNED_INT, 0);
    }

    pbr_terrain_shader.stop();

    pbr_shader.start();
    pbr_shader.load_projection_matrix();
    pbr_shader.load_camera_matrix(camera->get_camera_matrix());
    pbr_shader.load_camera_position(camera->get_position());
    pbr_shader.load_lights(lights);
    pbr_shader.load_light_space_matrix(shadowmap.get_light_position());
    pbr_shader.load_clipping_plane(clipping_plane);

    std::vector<std::unique_ptr<Model>> const& models {scene.get_models()};
    for (auto it = models.begin(); it != models.end(); it++)
    {
        std::vector<model::Vao_Data> const& vaos {(*it)->get_model_data()};

        for (model::Vao_Data const& model_data : vaos)
        {
            glBindVertexArray(model_data.vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, model_data.material.map_albedo);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, model_data.material.map_normal);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, model_data.material.map_metal);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, model_data.material.map_rough);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, model_data.material.map_ao);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, model_data.material.map_opacity);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            pbr_shader.load_has_opacity_map(model_data.material.has_opacity_map);


            pbr_shader.load_model_matrix((*it)->get_model_matrix());
            
            glDrawElements(GL_TRIANGLES, model_data.indices_count, GL_UNSIGNED_INT, 0);
        }
    }

    pbr_shader.stop();
}

void Renderer::render_wireframe(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, Scene const& scene) const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    render(camera,  lights, shadowmap, scene);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::render_to_shadowmap(Shadowmap shadowmap, Scene const& scene) const
{
    shadowmap.activate();
    glCullFace(GL_FRONT);
    shadowmap_shader.start();
    shadowmap_shader.load_light_space_matrix(shadowmap.get_light_position());

    auto const& road_models {scene.get_road().get_lanes()};
    for (auto const& lane : road_models)
    {
        model::Vao_Data const& model_data {lane.second->get_vao()};
        glBindVertexArray(model_data.vao);

        shadowmap_shader.load_model_matrix(mat4{});

        glDrawElements(GL_TRIANGLES, model_data.indices_count, GL_UNSIGNED_INT, 0);
    }

    std::vector<std::unique_ptr<Model>> const& models {scene.get_models()};
    for (auto it = models.begin(); it != models.end(); it++)
    {
        std::vector<model::Vao_Data> const& vaos {(*it)->get_model_data()};

        for (model::Vao_Data const& model_data : vaos)
        {
            glBindVertexArray(model_data.vao);

            shadowmap_shader.load_model_matrix((*it)->get_model_matrix());

            glDrawElements(GL_TRIANGLES, model_data.indices_count, GL_UNSIGNED_INT, 0);
        }
    }

    shadowmap_shader.stop();
    
    glCullFace(GL_BACK);
    shadowmap.deactivate();
}

void Renderer::render_sun(Framebuffer const& fbo, Light_Container const& lights, Camera const* camera) const
{
    fbo.bind();

    glDisable(GL_DEPTH_TEST);
    sun_shader.start();
    sun_shader.load_projection_matrix();
    sun_shader.load_camera_matrix(camera->get_camera_matrix().remove_translation());
    sun_shader.load_model_matrix(translation_matrix(lights.get_sun().get_position()) * lights.get_sun().get_rotation() * scale_matrix(5,5,5));
    vec3 color {255 / 255.0, 147 / 255.0, 41 / 255.0};
    sun_shader.load_color(color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lights.get_sun().get_vao().material.map_albedo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    glBindVertexArray(lights.get_sun().get_vao().vao);
    glDrawElements(GL_TRIANGLES, lights.get_sun().get_vao().indices_count, GL_UNSIGNED_INT, 0);
    sun_shader.stop();
    glEnable(GL_DEPTH_TEST);

    fbo.unbind();
}

void Renderer::render_godray(Framebuffer const& fbo, Camera const* camera, Scene const& scene) const
{
    fbo.bind();

    god_ray_shader.start();
    god_ray_shader.load_projection_matrix();
    god_ray_shader.load_camera_matrix(camera->get_camera_matrix());

    auto const& road_models {scene.get_road().get_lanes()};
    for (auto const& lane : road_models)
    {
        model::Vao_Data const& model_data {lane.second->get_vao()};
        glBindVertexArray(model_data.vao);

        god_ray_shader.load_model_matrix(mat4{});

        glDrawElements(GL_TRIANGLES, model_data.indices_count, GL_UNSIGNED_INT, 0);
    }

    std::vector<std::unique_ptr<Model>> const& models {scene.get_models()};
    for (auto it = models.begin(); it != models.end(); it++)
    {
        std::vector<model::Vao_Data> const& vaos {(*it)->get_model_data()};

        for (model::Vao_Data const& model_data : vaos)
        {
            glBindVertexArray(model_data.vao);

            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, model_data.material.map_opacity);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            pbr_shader.load_has_opacity_map(model_data.material.has_opacity_map);

            god_ray_shader.load_model_matrix((*it)->get_model_matrix());

            glDrawElements(GL_TRIANGLES, model_data.indices_count, GL_UNSIGNED_INT, 0);
        }
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

void Renderer::render_water(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, Water_Handler const& water, Scene const& scene) const
{
    auto & road_models {scene.get_road().get_lanes()};
    for (auto & lane : road_models)
    {
        Mud_Lane* mud_lane {dynamic_cast<Mud_Lane*>(lane.second.get())};
        if (mud_lane != nullptr)
        {
            model::Vao_Data const& water_surface {mud_lane->get_water_surface()};

            water_shader.start();
            water_shader.load_projection_matrix();
            water_shader.load_camera_matrix(camera->get_camera_matrix());
            water_shader.load_camera_position(camera->get_position());
            water_shader.load_lights(lights);
            water_shader.load_light_space_matrix(shadowmap.get_light_position());

            glActiveTexture(GL_TEXTURE10);
            glBindTexture(GL_TEXTURE_2D, shadowmap.get_texture_id());

            glBindVertexArray(water_surface.vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, water_surface.material.map_albedo);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, water_surface.material.map_normal);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, water_surface.material.map_metal);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, water_surface.material.map_rough);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, water_surface.material.map_ao);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glActiveTexture(GL_TEXTURE11);
            glBindTexture(GL_TEXTURE_2D, water.get_reflection_framebuffer().get_texture_id());
            glActiveTexture(GL_TEXTURE12);
            glBindTexture(GL_TEXTURE_2D, water.get_refraction_framebuffer().get_texture_id());
            glActiveTexture(GL_TEXTURE13);
            glBindTexture(GL_TEXTURE_2D, water.get_refraction_framebuffer().get_depth_id());

            water_shader.load_model_matrix(mat4{});
            
            glDrawElements(GL_TRIANGLES, water_surface.indices_count, GL_UNSIGNED_INT, 0);

            water_shader.stop();

            return;
        }
    }
}

//TODO: instance render 
