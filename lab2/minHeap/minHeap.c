#include "minHeap.h"

static void swapNodes(HfmNode** a, HfmNode** b) {
    HfmNode* t = *a;
    *a = *b;
    *b = t;
}

static void minHeapify(MinHeap* minHeap, size_t idx) {
    size_t smallest = idx;
    size_t left = 2 * idx + 1;
    size_t right = 2 * idx + 2;
    
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) {
        smallest = left;
    }
    
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
        smallest = right;
    }
    
    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

MinHeap* createMinHeap(size_t capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->array = (HfmNode**)malloc(sizeof(HfmNode*) * capacity);
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

void freeMinHeap(MinHeap* minHeap) {
    if (!minHeap) return;
    free(minHeap->array);
    free(minHeap);
}

void insertMinHeap(MinHeap* minHeap, HfmNode* node) {
    if (minHeap->size == minHeap->capacity) {
        fprintf(stderr, "MinHeap is full!\n");
        return;
    }

    size_t i = minHeap->size++;
    minHeap->array[i] = node;
    
    // 向上调整，保证父节点频率小于子节点
    while (i != 0 && minHeap->array[(i - 1) / 2]->freq > minHeap->array[i]->freq) {
        swapNodes(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

HfmNode* extractMin(MinHeap* minHeap) {
    if (minHeap->size == 0) return NULL;
    
    HfmNode* root = minHeap->array[0];
    
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    
    return root;
}

