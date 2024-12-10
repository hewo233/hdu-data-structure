#ifndef HFMTREE_H
#define HFMTREE_H

#include "../common.h"

HfmNode* buildHuffmanTreeImpl(const unsigned char *symbols, const size_t *freq, size_t size);
void printHuffmanTreeImpl(HfmNode *root, int depth);

int saveHuffmanTreeToFileImpl(HfmNode *root, FILE *fp);
HfmNode* loadHuffmanTreeFromFileImpl(FILE *fp);

#endif