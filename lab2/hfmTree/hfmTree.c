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
        printf("%c (%zu)\n", root->symbol, root->freq);
    else
        printf("* (%zu)\n", root->freq);
    
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
        // 叶子节点
        int symbol = fgetc(fp);
        if (symbol == EOF) return NULL;
        return createNode((unsigned char)symbol, 0); // 频率在还原树时可不重要
    } else if (flag == 0) {
        // 内部节点
        HfmNode* left = loadHuffmanTreeFromFileImpl(fp);
        HfmNode* right = loadHuffmanTreeFromFileImpl(fp);
        HfmNode* parent = createNode('\0', 0);
        parent->left = left;
        parent->right = right;
        return parent;
    } else {
        // 数据格式错误
        return NULL;
    }
}
