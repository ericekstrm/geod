#pragma once

#include "model_util.h"
#include "Bezier.h"
#include "Heightmap.h"

enum Driving_Direction {left, right, both, none};

class Lane
{
public:

    Lane(float width, Driving_Direction driving_direction);

    virtual model::Buffer_Data generate_vertex_data(Bezier const& bezier, float displacement) const = 0;

    float get_width() const { return width; }

    model::Material get_material() const { return material; }


    /**
     * Returns a list of all models that the lane need to look good.
     */
    virtual std::vector<model::Vao_Data> get_lane_models(Bezier const& bezier, float displacement) const = 0;

protected:

    model::Material material;

    /**
     * Generates the vertex data for the road surface, given a bezier to follow, a lateral 
     * displacement from the bezier, and a vector of heights.
     */
    model::Buffer_Data generate_vertex_data_help(Bezier const& bezier, float displacement, Heightmap height_data) const;

    static float get_quadratic_height(float normalized_point);

    //TODO: tmp variables (set algoritmicaly, somehow...)
    float nr_points {2};
    int nr_across {7}; // has to be odd!

private:

    float width;
    Driving_Direction driving_direction;

};

class Asphalt_Lane : public Lane
{
public:
    Asphalt_Lane(float width, Driving_Direction driving_direction);

    model::Buffer_Data generate_vertex_data(Bezier const& bezier, float displacement) const override;

    std::vector<model::Vao_Data> get_lane_models(Bezier const& bezier, float displacement) const override { return std::vector<model::Vao_Data>{}; }

private:

    float max_height {0.25};
};

class Mud_Lane : public Lane
{
public:
    Mud_Lane(float width, Driving_Direction driving_direction);

    model::Buffer_Data generate_vertex_data(Bezier const& bezier, float displacement) const override;

    std::vector<model::Vao_Data> get_lane_models(Bezier const& bezier, float displacement) const override;
};

class Ditch : public Lane
{
public:
    Ditch(float width);

    model::Buffer_Data generate_vertex_data(Bezier const& bezier, float displacement) const override;

    std::vector<model::Vao_Data> get_lane_models(Bezier const& bezier, float displacement) const override { return std::vector<model::Vao_Data>{}; }
};
