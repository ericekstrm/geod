#pragma once

#include "model_util.h"
#include "Bezier.h"
#include "Heightmap.h"

enum Driving_Direction {left, right, both, none};

class Lane
{
public:

    Lane(float width, Driving_Direction driving_direction, Bezier& bezier);

    virtual void generate_vertex_data(float displacement) = 0;

    float get_width() const { return width; }
    model::Vao_Data const& get_vao() const { return vao_data; }

    /**
     * Returns a list of all models that the lane need to look good.
     */
    virtual std::vector<model::Vao_Data> get_lane_models(float displacement) const = 0;

protected:

    /**
     * Generates the vertex data for the road surface, given a bezier to follow, a lateral 
     * displacement from the bezier, and a vector of heights.
     */
    model::Buffer_Data generate_vertex_data_help(float displacement, Lane_Heightmap height_data) const;

    static float get_quadratic_height(float normalized_point);

    float nr_points;
    int nr_across;


    //Lane parameters (TODO: move to a settings file)
    float mesh_grid_size {0.02}; //size of square in mesh in meters
    float tire_width {0.2};

    Bezier& bezier;
    
    float width;
    Driving_Direction driving_direction;
    model::Vao_Data vao_data;

};

class Asphalt_Lane : public Lane
{
public:
    Asphalt_Lane(float width, Driving_Direction driving_direction, Bezier& bezier);

    void generate_vertex_data(float displacement) override;

    std::vector<model::Vao_Data> get_lane_models(float displacement) const override { return std::vector<model::Vao_Data>{}; }

private:

    float max_height {0.25};
};

class Mud_Lane : public Lane
{
public:
    Mud_Lane(float width, Driving_Direction driving_direction, Bezier& bezier);

    void generate_vertex_data(float displacement) override;

    std::vector<model::Vao_Data> get_lane_models(float displacement) const override { return std::vector<model::Vao_Data>{}; }

    model::Vao_Data const& get_water_surface() const { return water_surface; }

private:

    model::Vao_Data water_surface;
    void generate_water_surface(float displacement);

    //Mud Lane parameters (TODO: move to a settings file)
    int erosion_iterations {200};
    int nr_potholes {50};
    float min_pothole_radius {0.2}; //radius in meter
    float max_pothole_radius {0.7}; //radius in meter

    float water_height {-0.15};
};

class Ditch : public Lane
{
public:
    Ditch(float width, Bezier& bezier);

    void generate_vertex_data(float displacement) override;

    std::vector<model::Vao_Data> get_lane_models(float displacement) const override;
};
