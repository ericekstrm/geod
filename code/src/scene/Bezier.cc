#include "Bezier.h"

#include <cmath>

Bezier::Bezier()
    : c1 {0,0}, c2 {40,0}, c3 {0,40}, c4 {40,40}, length {}
{

    //approximation for the length of the bezier curve
    for (int i = 0; i < steps; i++)
    {
        float u = i / static_cast<float>(steps);
        float v = (i + 1) / static_cast<float>(steps);
        length += (get_point(u) - get_point(v)).length();
    }
}

vec2 Bezier::get_point(float u) const
{
return c1 * pow(1 - u, 3) 
     + c2 * 3 * u * pow(1 - u, 2)
     + c3 * 3 * (1 - u) * pow(u, 2)
     + c4 * pow(u, 3);
}

vec2 Bezier::get_direction(float u) const
{
    return (c1 *  -3 * pow(1 - u, 2) 
          + c2 * ( 3 * pow(1 - u, 2) - 6 * u * (1 - u))
          + c3 * (-3 * pow(u    , 2) + 6 * (1 - u) * u)
          + c4 *   3 * pow(u    , 2)).normalize();
}

vec2 Bezier::get_perpendicular_direction(float u) const
{
    vec2 dir {get_direction(u)};
    return vec2{dir.y, -dir.x};
}

/*std::pair<vec2, vec2> Road_Bezier::get_edge_points(float u) const
{
    vec2 point {get_point(u)};
    vec2 direction {get_direction(u)};
    vec2 perpendicular_direction {direction.y, -direction.x};

    vec2 left_point  {point + perpendicular_direction * width / 2};
    vec2 right_point {point - perpendicular_direction * width / 2};

    return std::make_pair(left_point, right_point);

}

bool Road_Bezier::is_on(vec2 const& p) const
{
    for (int i = 0; i < steps; i++)
    {
        float u = i / static_cast<float>(steps);
        vec2 point = get_point(u);
        float l = (p - point).length();
        if (l < width / 2)
        {
            return true;
        }
    }

    return false;
}*/

float Bezier::distance_to(vec2 const& p) const
{
    float min_dist = (p - c1).length();
    for (int i = 0; i < steps; i++)
    {
        float u = i / static_cast<float>(steps);
        vec2 point = get_point(u);
        float l = (p - point).length();
        if (l < min_dist)
        {
            min_dist = l;
        }
    }

    return min_dist;
}