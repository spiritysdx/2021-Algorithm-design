//显示中文
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>//用于函数SetConsoleOutputCP(65001);更改cmd编码为utf8
#include <ctime>
//函数结果状态代码
#define         TRUE            1
#define         FALSE           0
#define          OK             1
#define          ERROR          0   
#define        INFEASIBLE       -1
#define         OVERFLOW        -2
#define  STACK_INIT_SIZE  100    //存储空间初始分配量
#define STACKINCREMENT 10    //存储空间分配增量
//宏常量
#define INFINITY INT_MAX   //最大值∞
#define  MAX_VERTEX_NUM 100   //最大顶点个数
#include<iostream>
using namespace std;


//设置状态码和数据类型
typedef int Status;
typedef int SElemType;
typedef struct 
{
	SElemType   	*base;  
    //数组首地址,在栈构造之前和销毁之后，base的值为NULL
	SElemType   	*top;   //栈顶指针
	int     	    stacksize;//当前已分配的存储空间，以元素为单位
}SqStack;

//初始化
Status InitStack(SqStack &S ) 
{
    // 构造一个空的顺序栈 
    S.base=new SElemType[STACK_INIT_SIZE];
    if (!S.base) 
    {
        exit(OVERFLOW);
    } //存储分配失败
    S.top = S.base; //长度为0
    S.stacksize = STACK_INIT_SIZE;//初始存储容量
    return OK;
}

//销毁
Status DestroyStack(SqStack &S)
{
    free(S.base);
    S.top == S.base == NULL;
    S.stacksize = 0;
    return OK;
}

//判栈空
Status StackEmpty(SqStack S)
{
	//若栈空，返回TRUE；否则返回FALSE。
	if(S.top==S.base) 
    {
        return TRUE; //栈空
	}
    else 
    {
    return FALSE;
    }
}

//求栈长
Status LengthStack(SqStack &S)
{
	return (S.top-S.base);//栈顶指针减去栈底指针
}

//清空栈
Status ClearStack(SqStack &S)
{
	S.top=S.base;//让栈顶指针指向栈底位置
	return OK; 
}


//读栈顶
Status GetTop(SqStack S, SElemType &e)
{
//若栈不空，则用e返回S的栈顶元素，并返OK，
//否则返回ERROR
    if(S.top==S.base) 
    {
        return ERROR; //栈空
    }
    e=*(S.top-1);
    return OK;
}



//进栈

//int
Status Push(SqStack &S, SElemType e)
{
//插入元素e为新的栈顶元素		
if(S.top-S.base >= S.stacksize) 
{//栈满，追加空间
    SElemType *newbase=(SElemType *)realloc(S.base, (S.stacksize+STACKINCREMENT)*sizeof(SElemType));
    if(!newbase) exit(OVERFLOW);
    S.base=newbase;
    S.top=S.base+S.stacksize;
    S.stacksize+=STACKINCREMENT; 
} //if
    *S.top++=e;	
    return OK;
}

//出栈
//int
Status Pop(SqStack &S, SElemType &e)
{
//若栈不空，则删除栈顶元素，用e返回其值，并返回OK；
//否则返回ERROR	
    if(S.top==S.base) 
    {
        e = *"$";
        return ERROR;  //栈空
    }
    e=*--S.top; 
    return OK;
}




typedef int Status;
typedef int VRType;
typedef struct{ } InfoType;
typedef enum{DG,DN,UDG,UDN} GraphKind;
typedef int VertexType; 

// 弧的定义
typedef struct ArcCell
{   
  VRType  adj; // VRType是顶点关系类型
  InfoType  *info;  // 该弧相关信息的指针
}ArcCell,AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];


typedef struct MGraph
{ // 图的定义
    VertexType vexs[MAX_VERTEX_NUM];//顶点向量
    AdjMatrix arcs;      // 邻接矩阵                     
    int vexnum, arcnum;  // 图的当前顶点数、弧数   
    int IncInfo;         //IncInfo为0,表示各弧无信息
    GraphKind   kind;    // 图的种类标志             
};

//基础操作函数

//清空图或网
void ClearGraph(MGraph &G)
{
  G.vexnum = 0;
  G.arcnum = 0;
  G.IncInfo = 0;
}

//查找顶点u位置
int LocateVex(MGraph G,VertexType u)
{
  for(int i=0;i!=G.vexnum;i++)
  {
    if(G.vexs[i] == u)
    {
      return i;
    }
  }
  return 0;
}

//返回第v个节点的值
VertexType GetVex(MGraph G,VertexType v)
{
  if(v>=1 && v<=G.vexnum)
  {
    return G.vexs[v];
  }
  else
  {
    return '\0';
  }
}

//对顶点v赋值
Status PutVex(MGraph &G,VertexType v,VertexType value)
{
  int k;
  k = LocateVex(G,v);
  if(k)
  {
    G.vexs[k]=value;
    return OK;
  }
  else
  {
    return ERROR;
  }
}

//查找第一邻接点序号
int FirstAdjVex(MGraph G,VertexType v)
{
  int k,j,t;
  k = LocateVex(G,v);

  if(k)
  {
    if(G.kind%2) //网
    {
      t = INFINITY;
    }
    else         //图
    {
      t = 0;
    }
    for(j=1;j<=G.vexnum;j++)
    {
      if(G.arcs[k][j].adj != t)
      {
        return j;
      }
    }
  }
  return 0;
}

//查找v相对于w的下一邻接点序号
int NextAdjVex(MGraph G,VertexType v,VertexType w)
{
  int k1,k2,j,t;

  k1 = LocateVex(G,v);
  k2 = LocateVex(G,w);

  if(k1 && k2)
  {
    if(G.kind%2)     //网
    {
      t = INFINITY;
    }
    else             //图
    { 
      t = 0;
    }

    for(j = k2+1;j<=G.vexnum;j++)
    {
      if(G.arcs[k1][j].adj != t)
      {
        return j;
      }
    }
  }
  return 0;
}



//录入弧信息
void InPut(InfoType *info)
{
  //弧信息为空
}

int randNum(int min_val,int max_val)
{
	int num_val = rand()%(max_val-min_val+1) + min_val;
	return num_val;
}


//构造有向图
Status CreateDG(MGraph &G)
{
  cout<<"输入有向图的点数："<<endl;
  cin>>G.vexnum;
  G.arcnum = G.vexnum*G.vexnum;
  
  G.IncInfo = 0;

  int i,j;
  for(i=0;i<G.vexnum;++i)
  {
    G.vexs[i] = i;     //构造顶点向量
  }
  for(i=0;i<G.vexnum;++i)   //初始化邻接矩阵
  {
    for(j=0;j<G.vexnum;++j)
    {
      G.arcs[i][j].adj = randNum(0,100);
      G.arcs[i][j].info= NULL;
    }
  }
  VertexType v1,v2;
  int k;

  for(i=0;i<G.vexnum;i++)
  {
    for(j=0;j<G.vexnum;j++)
    {
      if(i==j){
        G.arcs[i][j].adj = 0;
      }
      else{
        if(G.arcs[i][j].adj >= 90){
          G.arcs[i][j].adj = 1;
        }
        else{
          G.arcs[i][j].adj = 0;
        }
      }
    }
    

    if(G.IncInfo)
    {
      InPut(G.arcs[i][j].info);
    }
  }

  return OK;
}

//创建图或表
Status CreateGraph(MGraph &G) 
{ 
  return CreateDG(G);
}


void translateG(MGraph &G, MGraph &GT){
  GT.arcnum = G.arcnum;
  GT.vexnum = G.vexnum;
  int i,j;
  for(i=0;i<G.vexnum;++i)
  {
    GT.vexs[i] = i;     //构造顶点向量
  }
  for(i=0;i<G.vexnum;++i)   //初始化邻接矩阵
  {
    for(j=0;j<G.vexnum;++j)
    {
      GT.arcs[i][j].adj = INFINITE;
      GT.arcs[i][j].info= NULL;
    }
  }


	for(i=0;i<G.vexnum;++i)   
  	{
    	for(j=0;j<G.vexnum;++j)
    	{
        GT.arcs[i][j].adj = G.arcs[j][i].adj;
    	}
  	}
	return;
}


void DFS(MGraph &G ,int *visited, int v, SqStack &temp)  //从vex这个结点开始深度遍历图
{
    int w;
    visited[v]=1;
    cout<<G.vexs[v]<<"-->";
    Push(temp,G.vexs[v]);    //输出遍历的次序
    for(int j=0;j<G.vexnum;j++)   //循环遍历第v行，找到与v相邻的结点
    {
        if(G.arcs[v][j].adj == 1)  //表示v和j之间存在边
        {
            w=j;                 //此时w是v相邻的下一个结点
            if(visited[w]==0){    //表明这个相邻的结点还未被访问到
                DFS(G,visited,w,temp);//递归调用深度遍历函数
              }
        }
    }
}


void DFS_G(MGraph &G ,int *visited, int v, SqStack &temp)  //从vex这个结点开始深度遍历图
{
    int w;
    visited[v]=1;
    cout<<G.vexs[v]<<"-->";
    Push(temp,G.vexs[v]);    //输出遍历的次序
    for(int j=0;j<G.vexnum;j++)   //循环遍历第v行，找到与v相邻的结点
    {
        if( (G.arcs[v][j].adj == 1) & (G.arcs[j][v].adj == 1)  )//表示v和j之间存在边
        {
            w=j;   
            for(int k=0;k<G.vexnum;k++){ //从这个点的其他边不再被DFS搜寻
              G.arcs[v][k].adj = 0;
              //G.arcs[k][j].adj = 0;
              //G.arcs[j][k].adj = 0;
              G.arcs[k][v].adj = 0;
            }
            if(visited[w]==0){    //表明这个相邻的结点还未被访问到
                DFS(G,visited,w,temp);//递归调用深度遍历函数
              }
        }
    }
}


//打印函数
void Print(MGraph G)//输出图G的邻接矩阵
{
    for(int i=0;i!=G.vexnum;i++)
    {
      for(int j=0;j!=G.vexnum;j++)
      {
        if(G.arcs[i][j].adj != INFINITY)
        {
          cout<<G.arcs[i][j].adj<<'\t';
        }
        else
        {
          cout<<"∞"<<'\t';
        }
      }
      cout<<endl;
    }
}



//实验
void test()
{
  MGraph G,GT;
  bool result = CreateGraph(G);
  translateG(G,GT);
  if(result)
  {
	cout<<"正向矩阵G："<<endl;
  Print(G);
  
	cout<<"反向矩阵GT："<<endl;
	Print(GT);
  }


  SqStack S;
  InitStack(S);   //初始化栈S

  cout<<endl;
  for(int v=0;v!=G.vexnum;v++)
  {
    int visited[G.vexnum+1]={0};
    for(int x=0;x<=G.vexnum+1;x++)
    {
      visited[x] = 0;
    };
    SqStack temp;
    InitStack(temp);   //初始化栈temp存储每次DFS的次序
    DFS(GT,visited,v,temp);
    cout<<endl;
    int end_e = 10000;
    Pop(temp,end_e);
    Push(S,end_e);
    DestroyStack(temp);
    
  }
  cout<<"对G进行DFS"<<endl;

  int points[G.vexnum]={0};
  while (!StackEmpty(S))//从栈中取出结果，先进后出
    {   
      int visited[G.vexnum+1]={0};
      for(int x=0;x<=G.vexnum+1;x++)
      {
        visited[x] = 0;
      };
      SqStack temp1;
      InitStack(temp1);   //初始化栈temp存储每次DFS的次序
      int result = 100000;
      Pop(S,result);
      if(points[result]==0){
      //cout<<result<<"的强连通分支如为    ";
      DFS_G(G,visited,result,temp1);
      while(!StackEmpty(temp1)){
        int res = 100000;
        Pop(temp1,res);
        points[res]=1;
      }
      cout<<endl;
      DestroyStack(temp1);
      }
    }
  //cout<<"xxx"<<endl;
  for(int m=0;m<G.vexnum;m++)
  {
    if(points[m]!=1)
    {
      cout<<m<<"-->"<<endl;
    }
  }
  cout<<endl;
}

int main()
{
  //显示中文
  SetConsoleOutputCP(65001);

  //实验
  srand((unsigned)time(NULL));
  
  test();

  system("pause");
  return 0;
}




