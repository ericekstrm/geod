#include "Settings.h"

#include <iostream>
#include <filesystem>
#include <map>
#include <sstream>
#include <fstream>

vec2 Settings::window_size;

void Settings::load()
{
    set_defaults();

    std::ifstream ifs {"/opt/geod/settings.txt", std::ifstream::in};

    if (!ifs.good())
    {
        set_defaults();
        save();
        return;
    }

    std::map<std::string, std::string> values {};
    std::string str; 
    while (std::getline(ifs, str))
    {
        if (str.size() != 0)
        {
            auto pos = str.find_first_of(':');
            values[str.substr(0, pos)] = str.substr(pos + 1);
        }
    }

    window_size = str_to_vec2(values["window_size"]);
}

void Settings::set_defaults()
{
    window_size = vec2{920, 920};
}

void Settings::save()
{
    namespace fs = std::filesystem;
    fs::create_directories("/opt/geod");
    std::ofstream ofs {"/opt/geod/settings.txt", std::ofstream::out};

    ofs << "window_size:" << window_size << std::endl;

    ofs.close();
}

vec2 Settings::str_to_vec2(std::string const& str)
{
    auto pos = str.find_first_of(',');
    std::string x = str.substr(1, pos - 1);
    std::string y = str.substr(pos + 2);
    return vec2 {stof(x), stof(y.substr(0, y.size() - 1))};
}

// =========================
// ===| OpenGL Settings |===
// =========================

mat4 OpenGL_Settings::projection_matrix;

void OpenGL_Settings::load()
{
    projection_matrix = fov_projection_matrix(90.0f, Settings::get_window_width() / Settings::get_window_height(), 0.1f, 1000.0f);
}