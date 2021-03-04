#include "Scene.h"

Scene::Scene()
    : road{}
{

    std::unique_ptr<Model> m {std::make_unique<Model>("OBJ_JA19_TsugaDiversifolia_2")};
    m->set_scale(vec3(0.01, 0.01, 0.01));
    models.push_back(std::move(m));
}