#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct HfmNode {
    unsigned char symbol;
    size_t freq;
    struct HfmNode *left, *right;
} HfmNode;

#endif