#pragma once

#include "Model.h"
#include "model_util.h"
#include "Vector.h"
#include "Bezier.h"
#include "Lane.h"

#include <map>
#include <vector>
#include <memory>

class Road : public Model
{
public:
    Road();

    float get_total_width() const;

    // returns the with of a specific lane, or 0 if no lane exist at that index.
    float get_lane_width(int index) const;

    std::vector<model::Vao_Data> const& get_vao_data() const { return vao_data; }

private:

    Bezier bezier;

    std::map<int, std::unique_ptr<Lane>> lanes;

    std::vector<model::Vao_Data> vao_data;


    void generate_all_vertex_data();
    static model::Buffer_Data generate_vertex_data(Bezier const& bezier, Lane const& lane, float displacement);
};
