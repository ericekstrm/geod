#pragma once

#include "Vector.h"

class Road_Bezier
{
public:

    Road_Bezier();

    vec2 get_point(float u) const;
    vec2 get_direction(float u) const;

    std::pair<vec2, vec2> get_edge_points(float u) const;

    bool is_on(vec2 const& p) const;
    float distance_to(vec2 const& p) const;

    float get_width() const { return width; }
    float get_length() const { return length; }

private:

    //bezier control points
    vec2 c1;
    vec2 c2;
    vec2 c3;
    vec2 c4;

    float width;
    float length;

    int steps = 100; // nr of steps to take when iterating the curve. 
    //TODO: should be set as function of width and length. 

};
