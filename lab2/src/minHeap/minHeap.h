#ifndef MINHEAP_H
#define MINHEAP_H

#include "../common.h"

typedef struct MinHeap {
    HfmNode** array;
    size_t size;
    size_t capacity;
} MinHeap;


MinHeap* createMinHeap(size_t capacity);
void freeMinHeap(MinHeap* minHeap);
void insertMinHeap(MinHeap* minHeap, HfmNode* node);
HfmNode* extractMin(MinHeap* minHeap);


#endif