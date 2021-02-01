#pragma once

#include "Model.h"
#include "model_util.h"
#include "Vector.h"

class Road : public Model
{
public:
    Road();

    void generate_control_points();
    void generate_vertex_data();


private:

    vec2 bezier_point(float u);
    vec2 bezier_direction(float u);

    model::Buffer_Data data;

    vec2 control_points[4];

};