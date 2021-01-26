#include "settings.h"

int window_width {920};
int window_height {920};

mat4 projection {fov_projection_matrix(90.0f, window_width / window_height, 0.1f, 100.0f)};

int shadow_box_size {50};
mat4 light_projection {ortho_projection_matrix(-shadow_box_size, shadow_box_size, -shadow_box_size, shadow_box_size, 1.0f, 10000.0f)};

int terrain_size {512};
int terrain_resolution {512};
float terrain_max_height {20};