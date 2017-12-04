typedef struct HTNode
{
	int weight;
	int flag;
	int parent;
	int lchild;
	int rchild;
} HaffNode;
typedef struct code
{
	char bit[MaxBit];
	int start;
	int weight;
}Code;

void Haffman(HaffNode haffTree[],int weight[] ,int n)
{
	int i,j,m1,m2,x1,x2;
	for(i=0;i<2*n-1;i++)
	{
		if(i<n)
			haffTree[i].weight=weight[i];
		else
			haffTree[i].weight=0;
		haffTree[i].parent=-1;
		haffTree[i].flag=0;
		haffTree[i].lchild=-1;
		haffTree[i].rchild=-1;

	}
	for(i=0;i<n-1;i++)
	{
		m1=m2=MaxValue;
		x1=x2=0;
		for(j=0;j<n+i;j++)
		{
			if(haffTree[j].weight<m1&&haffTree[j].flag==0)
			{
				m2=m1;
				x2=x1;
				m1=haffTree[j].weight;
				x1=j;
			}
			else if(haffTree[j].weight<m2&&haffTree[j].flag==0)
			{
				m2=haffTree[j].weight;
				x2=j;
			}
		}
		haffTree[x1].parent=n+i;
		haffTree[x2].parent=n+i;
		haffTree[x1].flag=1;
		haffTree[x2].flag=1;
		haffTree[n+i].weight=haffTree[x1].weight+haffTree[x2].weight;
		haffTree[n+i].lchild=x1;
		haffTree[n+i].rchild=x2;
	}
}

void HaffmanCode(HaffNode haffTree[],int n,Code haffCode[])
{
	Code *cd=(Code *)malloc(sizeof(Code));
	int i,j,child,parent;
	for(i=0;i<n;i++)
	{
		cd->start=n-1;
		cd->weight=haffTree[i].weight;
		child=i;
		parent=haffTree[child].parent;
		while(parent!=-1)
		{
			if(haffTree[parent].lchild==child)
				cd->bit[cd->start]='0';
			else
				cd->bit[cd->start]='1';
			cd->start--;
			child=parent;
			parent=haffTree[child].parent;
		}
		for(j=cd->start+1;j<n;j++)
			haffCode[i].bit[j]=cd->bit[j];
		haffCode[i].start=cd->start+1;
		haffCode[i].weight=cd->weight;
	}
}


