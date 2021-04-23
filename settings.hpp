#pragma once

#include <stdint.h>

struct Settings {
    uint16_t screen_width;
    uint16_t screen_height;
    uint16_t map_width;
    uint16_t map_height;
    float scale;
    float drag;
    bool trails;
    float speed;
};
