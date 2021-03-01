#pragma once

#include "Road.h"

#include <memory>

class Scene
{
public:

    Scene();


    Road const& get_road() const { return road; }
    std::vector<std::unique_ptr<Model>> const& get_models() const { return models; }

private:

    Road road;

    std::vector<std::unique_ptr<Model>> models;
};