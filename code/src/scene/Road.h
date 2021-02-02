#pragma once

#include "Model.h"
#include "model_util.h"
#include "Vector.h"

class Road : public Model
{
public:
    Road();

    bool is_on(vec2 const& p) const;

    /**
     *  Calculates an aproximate closest distance from the the center
     *  of the road to a given point.
     * 
     */
    float distance_to(vec2 const& p) const;

private:

    void generate_bezier();
    void generate_vertex_data();

    vec2 bezier_point(float u) const;
    vec2 bezier_direction(float u) const;

    model::Buffer_Data data;

    vec2 control_points[4];

    float length;
    float width = 10;
};