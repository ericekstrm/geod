#pragma once

enum Driving_Direction {left, right, both};

class Lane
{
public:

    Lane(float width, Driving_Direction driving_direction);


    float get_width() const { return width; }

private:

    float width;
    Driving_Direction driving_direction;

};