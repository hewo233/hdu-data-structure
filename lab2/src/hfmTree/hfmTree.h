#ifndef HFMTREE_H
#define HFMTREE_H

#include "../common.h"

HfmNode* buildHuffmanTreeImpl(const unsigned char *symbols, const size_t *freq, size_t size);
void printHuffmanTreeImpl(HfmNode *root, int depth);

int saveHuffmanTreeToFileImpl(HfmNode *root, FILE *fp);
HfmNode* loadHuffmanTreeFromFileImpl(FILE *fp);

unsigned char* encodeDataImpl(HfmNode *root, const unsigned char *data, size_t dataSize, size_t *outSize);
unsigned char* decodeDataImpl(HfmNode *root, const unsigned char *encodedData, size_t encodedSize, size_t *outSize);

#endif