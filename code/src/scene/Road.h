#pragma once

#include "Model.h"
#include "model_util.h"
#include "Vector.h"
#include "Road_Bezier.h"

class Road : public Model
{
public:
    Road(Road_Bezier const& bezier);

private:

    static model::Buffer_Data generate_vertex_data(Road_Bezier const& bezier);
};
