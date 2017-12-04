typedef struct
{
	char aType[4];   // �ļ�����
	int nLength;     // ԭ�ļ�����
	int aWeight[256];// Ȩֵ��ֵ
}HEAD;

int Compress( char *pFilename)
{
	//��ȡ�ļ�����ʼ���ļ�ͷ
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
	printf("%d�ֽ�\n",sHead.nLength);
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
	free(myHaffTree);//�ͷ�Huffman���ռ�
	if(!myHaffCode)
	{
		return 0;
	}
	nSize = (nSize % 8)? nSize / 8 + 1 : nSize / 8;
	pBuffer=(char *)malloc(sizeof(char)*nSize);
	nSize=Encode(pFilename,myHaffCode,pBuffer, nSize);
	free(myHaffCode);//�ͷ�Huffman�����
	if(!pBuffer)
	{
		return 0;
	}
	//���ѹ���ļ�
	len = WriteFile(pFilename, sHead, pBuffer, nSize);
	free(pBuffer);
	if(len < 0)
	{
		return 0;
	}
	printf("%d�ֽ�\n",len);
	printf("ѹ������%lf%",(double)len/(double)sHead.nLength* 100);
	return 1;
}
int InitHead( char *pFilename, HEAD *Head)
{
	int ch;
	int i;
	FILE *in;
	const int SIZE = 256;
	//��ʼ���ļ�ͷ
	strcpy(Head->aType, "HUF");// �ļ�����
	Head->nLength = 0;//ԭ�ļ�����

	for(i = 0;i < SIZE;i++)
	{
		Head->aWeight[i] = 0;// Ȩֵ
	}

	//�Զ���������ʽ���ļ�
	in = fopen(pFilename, "rb");
	if(!in)
	{
		printf("��ʼ���ļ�ͷʧ�ܣ�");
		return 0;
	}

	//ɨ���ļ������Ȩ��
	while((ch = fgetc(in)) != -1)
	{
		Head->aWeight[ch]++;
		Head->nLength++;
	}
	//�ر��ļ����ͷ��ļ�ָ��
	fclose(in);
	in = NULL;
}
char Str2byte( char *pBinStr)
{
	char b = 0x00;
	int i;
	for(i = 0;i < 8;i++)
	{
		b = b << 1;//����1λ
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
	//�Զ���������ʽ���ļ�
	FILE *in = fopen(pFilename, "rb");
	if(!in)
	{
		printf("��Դ�ļ�ʧ�ܣ�");
		return 0;
	}
	//���ٻ�����
	 
	if(!pBuffer)
	{
		printf("���ٻ�����ʧ�ܣ�");
		return 0;
	}
	//ɨ���ļ�������HUffman�����������ѹ����ѹ������ݴ浽������
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
			//��ȡ�ַ�����ߵ�8���ַ���������ֽ�
			pBuffer[pos++] = Str2byte(cd);
			//�ַ�����������8�ֽ�
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
	//�����ļ���
	char filename[256] = {0};
	int len;
	FILE *out;
	strcpy(filename, pFilename);
	strcat(filename, ".huf");
	//�Զ���������ʽ���ļ�
	out = fopen(filename,"wb");
	//д�ļ�ͷ
	fwrite(&sHead, sizeof(HEAD), 1, out);
	//дѹ����ı���
	fwrite(pBuffer, sizeof(char), nSize, out);
	//�ر��ļ����ͷ��ļ�ָ��
	fclose(out);
	out = NULL;
	printf("����ѹ���ļ���%s\n",filename);
	len = sizeof(HEAD) + strlen(pFilename) + 1 + nSize;
	return len;
}