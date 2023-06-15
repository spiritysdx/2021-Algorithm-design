#include<bits/stdc++.h>
#define N 6000005
#define INF 1000000000
#define pr pair<long long,int>
using namespace std;
struct node
{
	int x,y,z;
}e[N];
int tot,n,m,head[N],d1[N],d2[N],nxt[N],in[N];
queue<int>q;
priority_queue<pr,vector<pr>,greater<pr> >qq;
bool vis[N];
long long ans=0,dis[N],d[N];
inline int read()
{
	int s=0,w=1;
	char ch=getchar();
	while (ch<'0'||ch>'9')
	{
		if (ch=='-')
			w=-1;
		ch=getchar();
	}
	while ('0'<=ch&&ch<='9')
	{
		s=s*10+ch-'0';
		ch=getchar();
	}
	return s*w;
}
inline void add(int x,int y,int z)
{
	tot++;
	d1[tot]=y;
	d2[tot]=z;
	nxt[tot]=head[x];
	head[x]=tot;
}
inline void spfa()
{
	for (int i=0;i<=n;i++)
		dis[i]=INF;
	dis[0]=0;
	vis[0]=true;
	q.push(0);
	while (!q.empty())
	{
		int u=q.front();
		q.pop();
		vis[u]=false;
		for (int i=head[u];i;i=nxt[i])
		{
			int v=d1[i];
			int cost=d2[i];
			if (dis[u]+cost<dis[v])
			{
				dis[v]=dis[u]+cost;
				if (!vis[v])
				{
					vis[v]=true;
					q.push(v);
					in[v]++;
					if (in[v]==n)
					{
						puts("-1");
						exit(0);
					}
				}
			}
		}
	}
}
void write(long long ans)
{
	if (!ans)
		return;
	if (ans<0)
		putchar('-'),ans=-ans;
	write(ans/10);
	putchar((char)(ans%10+48));
}
int main() 
{
	n=read(),m=read();
	for (int i=1;i<=m;i++)
	{
		e[i].x=read(),e[i].y=read(),e[i].z=read();
		add(e[i].x,e[i].y,e[i].z);
	}
	for (int i=1;i<=n;i++)
		add(0,i,0);
	spfa();
	for (int i=1;i<=m;i++)
	{
		e[i].z+=dis[e[i].x]-dis[e[i].y];
		d2[i]+=dis[e[i].x]-dis[e[i].y];
	}
	for (int i=1;i<=n;i++)
	{
		ans=0;
		for (int j=1;j<=n;j++)
			d[j]=INF,vis[j]=false;
		d[i]=0;
		while (!qq.empty())
			qq.pop();
		qq.push(make_pair(d[i],i));
		while (!qq.empty())
		{
			int u=qq.top().second;
			qq.pop();
			if (vis[u])
				continue;
			vis[u]=true;
			for (int i=head[u];i;i=nxt[i])
			{
				int v=d1[i];
				int cost=d2[i];
				if (d[u]+cost<d[v])
				{
					d[v]=d[u]+cost;
					qq.push(make_pair(d[v],v));
				}
			}
		}
		for (int j=1;j<=n;j++)
			if (d[j]==INF)
				ans=ans+(long long)j*INF; else
				ans=ans+(long long)j*(d[j]-dis[i]+dis[j]);
		if (!ans)
			putchar('0'); else
			write(ans);
		putchar('\n');
	}
	return 0;
}