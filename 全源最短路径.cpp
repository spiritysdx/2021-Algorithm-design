#include<iostream>
#include<cstring>
#define INF INT_MAX-100
#define MAX_VERtEX_NUM 20                   //顶点的最大个数
#define N 100
using namespace std;
typedef enum{undiscovered, visited}VStatus;
typedef struct
{
    VStatus vexstatus[MAX_VERtEX_NUM];
    int arc[MAX_VERtEX_NUM][MAX_VERtEX_NUM];
    int numV, numE;
}MGraph;
typedef struct Edge //边  
{  
    int u, v;  //两顶点
    int cost;  //权
}Edge;
class Graph
{
    private:
        MGraph G;
        Edge e[N];
        int pre[MAX_VERtEX_NUM];
        int dis[MAX_VERtEX_NUM];
        int mind[MAX_VERtEX_NUM];
    public:
        Graph()
        {
            int i, j;
            int nv, ne, a, b, w;
            cin >> nv >> ne;
            G.numV = nv;
            G.numE = ne;
            for (i = 0; i != nv; ++i)
                for (j = 0; j != nv; ++j)
                    G.arc[i][j] = INF;
            for (i = 0; i != ne; ++i)
            {
                cin >> a >> b >> w;
                G.arc[a-1][b-1] = w;
                e[i].u = a;
                e[i].v = b;
                e[i].cost = w;
            }
        }

        int numVex()
        {
            return G.numV;
        }
        int AddEdge()
        {
            int i, n;
            n = G.numE;
            for (i = G.numE-1; i >= 0; --i)
            {   
                e[i+G.numV].u = e[i].u;
                e[i+G.numV].v = e[i].v;
                e[i+G.numV].cost = e[i].cost;
            }
            for (i = 0; i != G.numV; ++i)
            {   
                e[i].u = 0;
                e[i].v = i+1;
                e[i].cost = 0;
                ++n;
            }
            return n;
        }
        void Bellman_ford()
        {
            int i, j;
            for(i = 1; i != G.numV+1; ++i) //初始化 
     	        dis[i] = INF;
            dis[0] = 0;
            bool p = 0;
            int t = AddEdge();
            for(i = 0; i != G.numV; ++i)  
            {
                p = 1;
                for(int j = 0; j != t; ++j)  
                    if(dis[e[j].v] > dis[e[j].u] + e[j].cost) //松弛（顺序一定不能反）  
                    {  
                        dis[e[j].v] = dis[e[j].u] + e[j].cost;  
                        p = 0;
                    }  
                if (p == 1)
                    break;
            }
        }
        void UpdateGraph()
        {
            int i, j;
            for(i = 0; i != G.numV; ++i)
                for(j = 0; j != G.numV; ++j)
                    if (G.arc[i][j] != INF)
                        G.arc[i][j] = G.arc[i][j]+dis[i+1]-dis[j+1];
        }
        void Dijkstra(int n)
        {
            int i, j, t = n, mi;
            for (i = 0; i != G.numV; ++i)
            {
                mind[i] = INF;
                G.vexstatus[i] = undiscovered;
                pre[i] = -1;
            }
            mind[n] = 0;
            G.vexstatus[n] = visited;
            for (i = 1; i != G.numV; ++i)
            {
                mi = INF;
                for (j = 0; j != G.numV; ++j)
                {
                    if ((G.arc[t][j] + mind[t] < mind[j]) && (G.vexstatus[j] != visited))
                    {
                        mind[j] = G.arc[t][j] + mind[t];
                        pre[j] = t;
                    }
                }
                for (j = 0; j != G.numV; ++j)
                    if ((mind[j] < mi) && (G.vexstatus[j] != visited))
                    {
                        t = j;
                        mi = mind[j];
                    }
                G.vexstatus[t] = visited;
            }
            cout << "以" << n+1 << "为源点至各点的最短路径为：";
            for (i = 0; i != G.numV; ++i)
                cout << mind[i] << ' ';
            cout << "以" << n+1 << "为源点时各点的前驱为(若无法直达，则输出-1)：";
            for (i = 0; i != G.numV; ++i)
                if (pre[i] == -1)
                    cout << pre[i] << ' ';
                else
                    cout << pre[i]+1 << ' ';            
            cout << endl;
        }
};
int main()
{
	Graph G;
    int i;
    G.Bellman_ford();
    G.UpdateGraph();
    for(i = 0; i != G.numVex(); ++i)
        G.Dijkstra(i);
	return 0;
}
