#pragma once

#include "event.h"

typedef struct {
    MenuLayer *layer;
    char **titles;
    char **subtitles;
    uint8_t count;
} ReleaseMenu;

void releases_init(char *query);