#include "Scene.h"


Scene::Scene()
    : bezier {}, road{bezier}
{

    for (int i = 0; i < 100; i++)
    {
        vec2 p {bezier.get_point(i / 100.0)};
        models.push_back(std::make_unique<Model>("grass", vec3{p.x, 1, p.y}));
    }

    for (int i = 0; i < 100; i++)
    {
        auto p {bezier.get_edge_points(i / 100.0)};

        models.push_back(std::make_unique<Model>("tree1", vec3{p.first.x, 1, p.first.y}));
        models.push_back(std::make_unique<Model>("tree1", vec3{p.second.x, 1, p.second.y}));
    }
}