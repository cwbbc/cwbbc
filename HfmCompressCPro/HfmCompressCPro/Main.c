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
	printf("-----------Huffman�ļ�ѹ��---------\n");
	printf("�������ļ���\n");
	scanf("%s",filename);
	Compress(filename);	
	return 0;
}