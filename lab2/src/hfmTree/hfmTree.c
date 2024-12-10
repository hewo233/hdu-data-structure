#include "hfmTree.h"
#include "../minHeap/minHeap.h"

static HfmNode* createNode(unsigned char symbol, size_t freq) {
    HfmNode* node = (HfmNode*)malloc(sizeof(HfmNode));
    node->symbol = symbol;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

HfmNode* buildHuffmanTreeImpl(const unsigned char *symbols, const size_t *freq, size_t size) {
    
    MinHeap* minHeap = createMinHeap(size);

    for (size_t i = 0; i < size; i++) {
        if (freq[i] > 0) {
            insertMinHeap(minHeap, createNode(symbols[i], freq[i]));
        }
    }
    
    while (minHeap->size > 1) {

        HfmNode* left = extractMin(minHeap);
        HfmNode* right = extractMin(minHeap);
        
        HfmNode* parent = createNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        
        insertMinHeap(minHeap, parent);
    }
    
    HfmNode* root = NULL;
    if (minHeap->size == 1) {
        root = extractMin(minHeap);
    }
    
    freeMinHeap(minHeap);
    return root;
}

void freeHuffmanTree(HfmNode* root) {
    if (!root) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

void printHuffmanTreeImpl(HfmNode *root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    if (root->symbol)
        printf("%c \n", root->symbol);
    else
        printf("* \n");
    
    printHuffmanTreeImpl(root->left, depth + 1);
    printHuffmanTreeImpl(root->right, depth + 1);
}

/*保存*/

int saveHuffmanTreeToFileImpl(HfmNode *root, FILE *fp) {
    if (!fp) return -1;
    if (!root) return 0;

    if (root->left == NULL && root->right == NULL) {

        fputc(1, fp);           
        fputc((unsigned char)root->symbol, fp);
    } else {
        
        fputc(0, fp);           
        saveHuffmanTreeToFileImpl(root->left, fp);
        saveHuffmanTreeToFileImpl(root->right, fp);
    }
    return 0;
}

HfmNode* loadHuffmanTreeFromFileImpl(FILE *fp) {
    if (!fp) return NULL;
    int flag = fgetc(fp);
    if (flag == EOF) return NULL;

    if (flag == 1) {
        
        int symbol = fgetc(fp);
        if (symbol == EOF) return NULL;
        return createNode((unsigned char)symbol, 0);
    } else if (flag == 0) {
        
        HfmNode* left = loadHuffmanTreeFromFileImpl(fp);
        HfmNode* right = loadHuffmanTreeFromFileImpl(fp);
        HfmNode* parent = createNode('\0', 0);
        parent->left = left;
        parent->right = right;
        return parent;
    } else {
        return NULL;
    }
}

/*编码译码*/

#define MAX_SYMBOLS 256

typedef struct {
    unsigned char *code; 
    size_t length;
} CodeTableEntry;

static void buildCodeTable(HfmNode *root, CodeTableEntry *table, unsigned char *buffer, size_t depth) {
    if (!root) return;
    
    if (!root->left && !root->right) {
        
        unsigned char *code = (unsigned char*)malloc(depth+1);
        for (size_t i = 0; i < depth; i++) {
            code[i] = buffer[i];
        }
        code[depth] = '\0';
        table[root->symbol].code = code;
        table[root->symbol].length = depth;
        return;
    }

    buffer[depth] = '0';
    buildCodeTable(root->left, table, buffer, depth+1);

    buffer[depth] = '1';
    buildCodeTable(root->right, table, buffer, depth+1);
}

static void freeCodeTable(CodeTableEntry *table) {
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (table[i].code) {
            free(table[i].code);
        }
    }
}


unsigned char* encodeDataImpl(HfmNode *root, const unsigned char *data, size_t dataSize, size_t *outSize) {
    if (!root || !data || dataSize == 0) {
        *outSize = 0;
        return NULL;
    }

    CodeTableEntry table[MAX_SYMBOLS] = {0};
    unsigned char buffer[1024];
    buildCodeTable(root, table, buffer, 0);

    size_t totalBits = 0;
    for (size_t i = 0; i < dataSize; i++) {
        if (table[data[i]].length == 0) {
            fprintf(stderr, "Symbol '%c' not found in the Huffman tree.\n", data[i]);
            freeCodeTable(table);
            return NULL;
        }
        totalBits += table[data[i]].length;
    }

    size_t byteCount = (totalBits + 7) / 8;
    size_t headerSize = sizeof(size_t); 
    unsigned char *encoded = (unsigned char*)calloc(byteCount + headerSize, 1); 
    if (!encoded) {
        fprintf(stderr, "Failed to allocate memory for encoded data.\n");
        freeCodeTable(table);
        return NULL;
    }

    size_t originalSize = dataSize;
    for (size_t i = 0; i < headerSize; i++) {
        encoded[i] = (originalSize >> (8 * (headerSize - 1 - i))) & 0xFF;
    }

    size_t bitPos = 0;
    for (size_t i = 0; i < dataSize; i++) {
        unsigned char *code = table[data[i]].code;
        size_t length = table[data[i]].length;
        for (size_t j = 0; j < length; j++) {
            size_t totalBitPos = bitPos;
            size_t byteIndex = headerSize + (totalBitPos / 8);
            size_t bitIndex = totalBitPos % 8;
            if (code[j] == '1') {
                encoded[byteIndex] |= (1 << (7 - bitIndex));
            }
            bitPos++;
        }
    }

    *outSize = byteCount + headerSize;
    freeCodeTable(table);
    return encoded;
}


unsigned char* decodeDataImpl(HfmNode *root, const unsigned char *encodedData, size_t encodedSize, size_t *outSize) {
    if (!root || !encodedData || encodedSize <= sizeof(size_t)) {
        *outSize = 0;
        return NULL;
    }

    size_t headerSize = sizeof(size_t);
    size_t originalDataSize = 0;
    for (size_t i = 0; i < headerSize; i++) {
        originalDataSize = (originalDataSize << 8) | encodedData[i];
    }

    if (originalDataSize == 0) {
        *outSize = 0;
        return NULL;
    }

    size_t totalBits = (encodedSize - headerSize) * 8;

    unsigned char *decoded = (unsigned char*)malloc(originalDataSize);
    if (!decoded) {
        fprintf(stderr, "Failed to allocate memory for decoded data.\n");
        return NULL;
    }
    size_t decIndex = 0;

    HfmNode *current = root;
    for (size_t bitPos = 0; bitPos < totalBits && decIndex < originalDataSize; bitPos++) {
        size_t byteIndex = headerSize + (bitPos / 8);
        size_t bitIndex = bitPos % 8;
        int bit = (encodedData[byteIndex] & (1 << (7 - bitIndex))) ? 1 : 0;

        current = bit == 0 ? current->left : current->right;

        if (!current->left && !current->right) {
            decoded[decIndex++] = current->symbol;
            current = root;

            if (decIndex == originalDataSize) {
                break;
            }
        }
    }

    *outSize = decIndex;
    return decoded;
}
