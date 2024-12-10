#include "common.h"
#include "hfmTree/hfmTree.h"
#include <stdio.h>

#define MAXN 256

int main()
{

    int mode;
    printf("输入模式(0 表示更新数据,1表示直接从文件读取): ");
    scanf("%d", &mode);

    HfmNode* root = NULL;

    if(mode == 0)
    {
        unsigned char symbols[MAXN];
        size_t freq[MAXN];
        int n;
        printf("输入个数: ");
        scanf("%d", &n);

        for(int i = 0; i < n; i++) {
            printf("输入字符: ");
            scanf(" %c", &symbols[i]);
            printf("输入频率: ");
            scanf("%zu", &freq[i]);
        }

        root = buildHuffmanTreeImpl(symbols, freq, n);
        FILE *fp = fopen("file/hfmTree", "w+");
        saveHuffmanTreeToFileImpl(root, fp);
    }  
    else 
    {
        FILE *fp = fopen("file/hfmTree", "r");
        root = loadHuffmanTreeFromFileImpl(fp);
    }

    printf("\nHuffman Tree:\n");

    printHuffmanTreeImpl(root, 0);
}