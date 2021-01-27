#pragma once

#include "Model.h"
#include "model_util.h"
#include "Vector.h"

class Road : public Model
{
public:
    Road();

    void generate_road();


private:
    model::Buffer_Data data;

};