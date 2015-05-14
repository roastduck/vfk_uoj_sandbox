#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

inline void READ(int &x)
{
	char c;
	x=0;
	do c=getchar(); while (c<'0' || c>'9');
	do x=x*10+c-48, c=getchar(); while (c>='0' && c<='9');
}

typedef long long ll;

const int maxn=1e5+5;

struct PATH
{
	int color, v;
	PATH() {}
	PATH(int _color, int _v) : color(_color), v(_v) {}
};

int N, m, a[maxn], b[maxn<<1], c[maxn<<1], color[maxn], rank[maxn], size[maxn];
ll fnt[maxn], bck[maxn];
bool forb[maxn];
PATH path[maxn];

void findSize(int x, int father)
{
	size[x]=1;
	for (int i=a[x];i;i=c[i]) if (b[i]!=father && !forb[b[i]])
	{
		findSize(b[i],x);
		size[x]+=size[b[i]];
	}
}

int findRoot(int x, int father, int overall)
{
	int ret=size[x]*2>=overall?x:0;
	for (int i=a[x];i;i=c[i]) if (b[i]!=father && !forb[b[i]])
	{
		int got=findRoot(b[i],x,overall);
		if (got) return got;
		if (size[b[i]]*2>overall) ret=0;
	}
	return ret;
}

void Dfs(int x, int father, int COLOR, int &n)
{
	path[n++]=PATH(COLOR,rank[x]);
	for (int i=a[x];i;i=c[i]) if (b[i]!=father && !forb[b[i]])
		Dfs(b[i],x,COLOR+color[b[i]],n);
}

inline bool cmpmax(const PATH &a, const PATH &b) { return a.v<b.v; }
inline bool cmpmin(const PATH &a, const PATH &b) { return a.v>b.v; }

int stamp;

struct TANK
{
	int data[maxn*2], tag[maxn*2];
	int &operator[](int id)
	{
		id+=maxn;
		if (tag[id]!=stamp) tag[id]=stamp, data[id]=0;
		return data[id];
	}
};

void Stat(int begin, int end, ll f[], bool(*cmp)(const PATH&,const PATH&), int k, int reduce)
{
	static TANK tank;
	stamp++;
	sort(path+begin,path+end,cmp);
	for (int i=begin;i<end;i++)
	{
		f[path[i].v]+=tank[reduce-path[i].color];
		tank[path[i].color]+=k;
	}
}

int main()
{
	READ(N);
	for (int i=1;i<=N;i++)
	{
		READ(color[i]);
		if (!color[i]) color[i]=-1;
	}
	for (int i=1;i<N;i++)
	{
		int x, y;
		READ(x), READ(y);
		m++, b[m]=y, c[m]=a[x], a[x]=m;
		m++, b[m]=x, c[m]=a[y], a[y]=m;
	}
	for (int i=1;i<=N;i++)
	{
		int x;
		READ(x);
		rank[x]=i;
	}
	static int list[maxn];
	int st, en;
	list[st=en=1]=N/2+1;
	for (;st<=en;st++)
	{
		int ori(list[st]);
		findSize(ori,0);
		int x=findRoot(ori,0,size[ori]), n=0;
		for (int i=a[x];i;i=c[i]) if (!forb[b[i]])
		{
			int _n(n);
			Dfs(b[i],x,color[b[i]]+color[x],n);
			Stat(_n,n,fnt,cmpmin,-1,color[x]);
			Stat(_n,n,bck,cmpmax,-1,color[x]);
			list[++en]=b[i];
		}
		path[n++]=PATH(color[x],rank[x]);
		Stat(0,n,fnt,cmpmin,1,color[x]);
		Stat(0,n,bck,cmpmax,1,color[x]);
		forb[x]=true;
	}
	for (int i=2;i<=N;i++)
	{
		fnt[i]+=fnt[i-1];
		bck[i]+=bck[i-1];
	}
	ll ans=0;
	for (int i=1;i<=N;i++)
	{
		int l(0), r(i-1), ret(-1);
		while (l<=r)
		{
			int mid((l+r)>>1);
			if (bck[i]-fnt[mid]-bck[mid]-fnt[N]+fnt[i]>0)
				l=mid+1, ret=max(ret,mid);
			else
				r=mid-1;
		}
		ans+=ret+1;
	}
	cout << ans << endl;
	return 0;
}
