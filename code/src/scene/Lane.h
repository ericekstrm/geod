#pragma once

#include "model_util.h"

enum Driving_Direction {left, right, both, none};

class Lane
{
public:

    Lane(float width, Driving_Direction driving_direction);


    float get_width() const { return width; }

    model::Material get_material() const { return material; }

protected:

    model::Material material;

private:

    float width;
    Driving_Direction driving_direction;

};

class Asphalt_Lane : public Lane
{
public:
    Asphalt_Lane(float width, Driving_Direction driving_direction);
};

class Ditch : public Lane
{
public:
    Ditch(float width);
};