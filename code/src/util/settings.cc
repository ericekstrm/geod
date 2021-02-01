#include "settings.h"

int shadow_box_size {50};
mat4 light_projection {ortho_projection_matrix(-shadow_box_size, shadow_box_size, -shadow_box_size, shadow_box_size, 1.0f, 10000.0f)};
