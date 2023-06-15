//显示中文
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>//用于函数SetConsoleOutputCP(65001);更改cmd编码为utf8
#include <bits/stdc++.h>
using namespace std;

const int N = 5;
int final_path[N+1]; //存储最终的路径
bool visited[N]; //记录已经访问过的节点
int final_res = INT_MAX; //存储最短行程的最终最小权重

int randNum(int min_val,int max_val)
{
	int num_val = rand()%(max_val-min_val+1) + min_val;
	return num_val;
}

//将临时路径复制到最终的路径
void copyToFinal(int curr_path[])
{
	for (int i=0; i<N; i++){
		final_path[i] = curr_path[i];
	}
	final_path[N] = curr_path[0];
}

//寻找权重最小边的函数
//在顶点i处结束
int firstMin(int adj[N][N], int i)
{
	int min = INT_MAX;
	for (int k=0; k<N; k++){
		if (adj[i][k]<min && i != k){
			min = adj[i][k];
		}
	}
	return min;
}

//寻找权重第二小边的函数
//在顶点i处结束
int secondMin(int adj[N][N], int i)
{
	int first = INT_MAX, second = INT_MAX;
	for (int j=0; j<N; j++)
	{
		if (i == j){
			continue;
		}
		if (adj[i][j] <= first)
		{
			second = first;
			first = adj[i][j];
		}
		else if (adj[i][j] <= second && adj[i][j] != first){
			second = adj[i][j];
		}
	}
	return second;
}


//curr_bound 根节点的下界
//curr_weight 存储路径的权重到此为止
//level 在搜索中移动时的层级
//curr_path[] 存储当前解决方案，会被复制到final_path中
void TSPRec(int adj[N][N], int curr_bound, int curr_weight, int level, int curr_path[])
{
	// 当达到第N级时意味着当前路径已经覆盖了所有节点一次
	if (level==N)
	{
		//检查最后一个顶点是否有边，返回第一个顶点的路径
		if (adj[curr_path[level-1]][curr_path[0]] != 0)
		{
			//curr_res 总权重
			int curr_res = curr_weight + adj[curr_path[level-1]][curr_path[0]];

			//如果当前结果较好更新最终路径
			if (curr_res < final_res)
			{
				copyToFinal(curr_path);
				final_res = curr_res;
			}
		}
		return;
	}

	//对于其他层级，迭代所有顶点递归构建搜索最小生成树
	for (int i=0; i<N; i++)
	{
		// 下一个顶点如果和初始顶点不相同考虑下一个顶点
		if (adj[curr_path[level-1]][i] != 0 &&
			visited[i] == false)
		{
			int temp = curr_bound;
			curr_weight += adj[curr_path[level-1]][i];

			//第1层级和其他层级下限计算方式不同
			if (level==1){
				curr_bound -= ((firstMin(adj, curr_path[level-1]) + firstMin(adj, i))/2);
			}
			else{
				curr_bound -= ((secondMin(adj, curr_path[level-1]) + firstMin(adj, i))/2);
			}

			// curr_bound + curr_weight 是实际的下限
			// 对于当前节点，如果下限 < final_res，需要继续向下走
			if (curr_bound + curr_weight < final_res)
			{
				curr_path[level] = i;
				visited[i] = true;

				//在下一层级调用TSPRec 递归
				TSPRec(adj, curr_bound, curr_weight, level+1,
					curr_path);
			}

			// 不满足条件节点通过重置来修剪节点 去除 curr_weight 和 curr_bound 的更新
			curr_weight -= adj[curr_path[level-1]][i];
			curr_bound = temp;

			// 重置记录访问的数组
			memset(visited, false, sizeof(visited));
			for (int j=0; j<=level-1; j++){
				visited[curr_path[j]] = true;
			}
		}
	}
}

//对final_path[]进行操作
void TSP(int adj[N][N])
{
	int curr_path[N+1];

	// 计算根节点的初始下界，同时初始化curr_path和记录访问的数组
	int curr_bound = 0;
	memset(curr_path, -1, sizeof(curr_path));
	memset(visited, 0, sizeof(curr_path));

	//计算初始边界
	for (int i=0; i<N; i++){
		curr_bound += (firstMin(adj, i) +secondMin(adj, i));
	}

	//将下限四舍五入为整数
	curr_bound = (curr_bound&1)? curr_bound/2 + 1 :curr_bound/2;

	visited[0] = true;
	curr_path[0] = 0;

	//开始搜索
	TSPRec(adj, curr_bound, 0, 1, curr_path);
}


int main()
{

    //显示中文
    SetConsoleOutputCP(65001);

	//随机算子
	srand((unsigned)time(NULL));

	//给定图的邻接矩阵
	int adj[N][N];
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			adj[i][j]=randNum(1,100);
			if(i==j){
				adj[i][j] = 200;
			}
			else{
				adj[j][i] = adj[i][j];
			}
		}
	}

	//输出矩阵
	cout<<"邻接矩阵："<<endl;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			cout <<adj[i][j]<<"   ";
		}
		cout << endl;
	}

	TSP(adj);

	printf("最短路径权重: %d\n", final_res);
	printf("最短路径路径: ");
	for (int i=0; i<=N; i++){
		printf("%d ", final_path[i]);
	}

	printf("\n");
	system("pause");
	return 0;
}


//https://www.geeksforgeeks.org/traveling-salesman-problem-using-branch-and-bound-2/2
