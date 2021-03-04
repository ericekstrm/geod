#pragma once

#include "Model.h"
#include "model_util.h"
#include "Vector.h"
#include "Bezier.h"
#include "Lane.h"

#include <map>
#include <vector>
#include <memory>

class Road
{
public:
    Road();

    float get_total_width() const;

    // returns the with of a specific lane, or 0 if no lane exist at that index.
    float get_lane_width(int index) const;

    std::map<int, std::unique_ptr<Lane>> const& get_lanes() const { return lanes; }

private:

    Bezier bezier;

    std::map<int, std::unique_ptr<Lane>> lanes;

    void generate_all_vertex_data();
};
