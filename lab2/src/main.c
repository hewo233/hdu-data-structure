#include "common.h"
#include "hfmTree/hfmTree.h"
#include <complex.h>
#include <stdio.h>

#define MAXN 256

void INIT()
{
    printf("INIT\n");

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

    HfmNode *buildRoot = buildHuffmanTreeImpl(symbols, freq, n);
    FILE *fp = fopen("file/hfmTree", "w+");
    saveHuffmanTreeToFileImpl(buildRoot, fp);
    fclose(fp);
    
}

HfmNode *root;
size_t dataSize, encodedSize, decodedSize;

void Encode()
{
    printf("Encode\n");

    FILE *fp = fopen("file/hfmTree", "r");
    root = loadHuffmanTreeFromFileImpl(fp);
    fclose(fp);

    FILE *tbt_fp = fopen("file/ToBeTran", "r");

    unsigned char data[MAXN];
    size_t dataSize = fread(data, 1, MAXN, tbt_fp);
    fclose(tbt_fp);

    unsigned char *encoded = encodeDataImpl(root, data, dataSize, &encodedSize);

    FILE *cf_fp = fopen("file/CodeFile", "w+");
    fwrite(encoded, 1, encodedSize, cf_fp);
    fclose(cf_fp);

}

void Print()
{
    FILE *cf_fp = fopen("file/CodeFile", "r");
    unsigned char encoded[MAXN];
    fread(encoded, 1, MAXN, cf_fp);
    fclose(cf_fp);

    printf("Encode result\n");

    printf("The size of encoded data: %zu\n", encodedSize);
    printf("Encoded data in binary format: \n");

    for(size_t i = 8; i < encodedSize; i++) {
        for(int j = 7; j >= 0; j--) {
            printf("%d", (encoded[i] >> j) & 1);
        }
    }
    printf("\n");

}

void Decode()
{
    FILE *cf_fp = fopen("file/CodeFile", "r");

    unsigned char encoded[MAXN];
    fread(encoded, 1, MAXN, cf_fp);
    fclose(cf_fp);

    unsigned char *decoded = decodeDataImpl(root, encoded, encodedSize, &decodedSize);

    printf("decoded data: \n");
    for(size_t i = 0; i < decodedSize; i++) {
        printf("%c", decoded[i]);
    }
    printf("\n");

    FILE *tf_fp = fopen("file/TextFile", "w+");
    fwrite(decoded, 1, decodedSize, tf_fp);
    fclose(tf_fp);
}

void TreePrint()
{
    printHuffmanTreeImpl(root, 0);
}

int main()
{
    char input_c;
    do
    {
        scanf("%c", &input_c);
        switch(input_c)
        {
            case 'I':
                INIT();
                break;
            case 'E':
                Encode();
                break;
            case 'P':
                Print();
                break;
            case 'D':
                Decode();
                break;
            case 'T':
                TreePrint();
                break;
            case 'Q':
                printf("Quit\n");
                return 0;
        }
    }
    while(1);
}