//#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Camera.h"

#include "Light.h"

#include "Model.h"

#include "Shader.h"
#include "Model_Shader.h"


TEST_CASE("Camera")
{
    SECTION("Base class")
    {
        Camera c {};
        REQUIRE(c.get_position() == vec3{5,2,5});
    }
}

TEST_CASE("Gui")
{

}

TEST_CASE("Light")
{
    Light_Container lights {};
    REQUIRE(lights.get_number_of_lights() == 1);

    lights.add_dir_light(vec3{10,10,10}, vec3{0,1,1});
    lights.add_dir_light(vec3{15,10,10}, vec3{0,1,1});
    lights.add_dir_light(vec3{20,10,10}, vec3{0,1,1});
    lights.add_dir_light(vec3{25,10,10}, vec3{0,1,1});
    lights.add_dir_light(vec3{30,10,10}, vec3{0,1,1});

    REQUIRE(lights.get_number_of_lights() == 6);
}

TEST_CASE("Model")
{
    SECTION("Copy")
    {
        Model m {"rock1"};
    }
}

TEST_CASE("Shader")
{
    SECTION("Base class")
    {
        Shader shader {};
    }

    SECTION("Model shader")
    {
        Model_Shader shader {};
        Model_Shader shader2 {};

    }
    
    
}

TEST_CASE("State")
{
    
}

TEST_CASE("Tree")
{
    
}

TEST_CASE("Util")
{
    
}