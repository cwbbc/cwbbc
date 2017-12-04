typedef struct
{
	char aType[4];   // 文件类型
	int nLength;     // 原文件长度
	int aWeight[256];// 权值数值
}HEAD;

int Compress( char *pFilename)
{
	//读取文件，初始化文件头
	HEAD sHead;
	int n=256;
	int nSize = 0;
	int i,j;
	int len;
	char *pBuffer=NULL;
	HaffNode *myHaffTree=(HaffNode *)malloc(sizeof(HaffNode)*(2*n-1));
	Code *myHaffCode=(Code *)malloc(sizeof(Code)*n);
	if(InitHead(pFilename, &sHead))
	{
		return 0;
	}
	printf("%d字节\n",sHead.nLength);
	Haffman(myHaffTree,sHead.aWeight,n);
	if(!myHaffTree)
	{
		return 0;
	}
	HaffmanCode(myHaffTree,n,myHaffCode);
	for(i = 0;i < 256;i++)
	{
		
		nSize += sHead.aWeight[i] * (n-myHaffCode[i].start);
	}
	free(myHaffTree);//释放Huffman树空间
	if(!myHaffCode)
	{
		return 0;
	}
	nSize = (nSize % 8)? nSize / 8 + 1 : nSize / 8;
	pBuffer=(char *)malloc(sizeof(char)*nSize);
	nSize=Encode(pFilename,myHaffCode,pBuffer, nSize);
	free(myHaffCode);//释放Huffman编码表
	if(!pBuffer)
	{
		return 0;
	}
	//输出压缩文件
	len = WriteFile(pFilename, sHead, pBuffer, nSize);
	free(pBuffer);
	if(len < 0)
	{
		return 0;
	}
	printf("%d字节\n",len);
	printf("压缩比率%lf%",(double)len/(double)sHead.nLength* 100);
	return 1;
}
int InitHead( char *pFilename, HEAD *Head)
{
	int ch;
	int i;
	FILE *in;
	const int SIZE = 256;
	//初始化文件头
	strcpy(Head->aType, "HUF");// 文件类型
	Head->nLength = 0;//原文件长度

	for(i = 0;i < SIZE;i++)
	{
		Head->aWeight[i] = 0;// 权值
	}

	//以二进制流形式打开文件
	in = fopen(pFilename, "rb");
	if(!in)
	{
		printf("初始化文件头失败！");
		return 0;
	}

	//扫描文件，获得权重
	while((ch = fgetc(in)) != -1)
	{
		Head->aWeight[ch]++;
		Head->nLength++;
	}
	//关闭文件，释放文件指针
	fclose(in);
	in = NULL;
}
char Str2byte( char *pBinStr)
{
	char b = 0x00;
	int i;
	for(i = 0;i < 8;i++)
	{
		b = b << 1;//左移1位
		if(pBinStr[i] == '1')
		{
			b = b | 0x01;
		}
	}
	return b;
}
int Encode( char *pFilename, Code pHC[], char pBuffer[],  int nSize)
{ 
	
	int SIZE=256;
	int ch;
	int i,j;
	int pos=0;
	char cd[256] = {'\0'};
	//以二进制流形式打开文件
	FILE *in = fopen(pFilename, "rb");
	if(!in)
	{
		printf("打开源文件失败！");
		return 0;
	}
	//开辟缓冲区
	 
	if(!pBuffer)
	{
		printf("开辟缓冲区失败！");
		return 0;
	}
	//扫描文件，根据HUffman编码表对其进行压缩，压缩结果暂存到缓冲区
	while((ch = fgetc(in)) != EOF)
	{
		int c=0;
	    for(j=pHC[ch].start;j<SIZE;j++)
		{
			cd[c]=pHC[ch].bit[j];
			c++;
		}
		while(strlen(cd) >= 8)
		{
			//截取字符串左边的8个字符，编码成字节
			pBuffer[pos++] = Str2byte(cd);
			//字符串整体左移8字节
			for(i = 0;i < SIZE -8;i++)
			{
				cd[i] = cd[i + 8];
			}
		}
	}
	if(strlen(cd) > 0)
	{
		pBuffer[pos++] = Str2byte(cd);
	}
	return pos;
}

int WriteFile( char *pFilename, HEAD sHead,  char *pBuffer, int nSize)
{
	//生成文件名
	char filename[256] = {0};
	int len;
	FILE *out;
	strcpy(filename, pFilename);
	strcat(filename, ".huf");
	//以二进制流形式打开文件
	out = fopen(filename,"wb");
	//写文件头
	fwrite(&sHead, sizeof(HEAD), 1, out);
	//写压缩后的编码
	fwrite(pBuffer, sizeof(char), nSize, out);
	//关闭文件，释放文件指针
	fclose(out);
	out = NULL;
	printf("生成压缩文件：%s\n",filename);
	len = sizeof(HEAD) + strlen(pFilename) + 1 + nSize;
	return len;
}