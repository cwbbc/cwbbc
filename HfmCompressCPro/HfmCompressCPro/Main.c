#define MaxValue 1000000
#define MaxBit 256
#include<stdio.h>
#include<stdlib.h>
#include<String.h>
#include"Huffman.h"
#include"Compress.h"
int main()
{
	char filename[256];
	printf("-----------Huffman文件压缩---------\n");
	printf("请输入文件名\n");
	scanf("%s",filename);
	Compress(filename);	
	return 0;
}