#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>
typedef struct Bullet {
    Vector2 bullet_position;
    int bullet_radius;
    Color bullet_color;
} Bullet;