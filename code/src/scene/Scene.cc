#include "Scene.h"

#include <filesystem>
namespace fs = std::filesystem;

Scene::Scene()
    : road{}
{

    //commented code because bezier is moved into Road.
    /*for (int i = 0; i < 10; i++)
    {
        vec2 p {bezier.get_point(i / 10.0)};
        models.push_back(std::make_unique<Model>("grass", vec3{p.x, 1, p.y}));
    }

    for (int i = 0; i < 10; i++)
    {
        auto p {bezier.get_edge_points(i / 10.0)};

        std::string name1 {"tree" + std::to_string(rand() % 12 + 1)};
        std::string name2 {"tree" + std::to_string(rand() % 12 + 1)};
        models.push_back(std::make_unique<Model>(name1, vec3{p.first.x, 1, p.first.y}));
        models.push_back(std::make_unique<Model>(name2, vec3{p.second.x, 1, p.second.y}));
    }*/
}