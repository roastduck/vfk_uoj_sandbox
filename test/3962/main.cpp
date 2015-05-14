#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

typedef long long ll;

const int maxn=1e5+5;

struct SUB
{
	int l, r;
	SUB() {}
	SUB(int _l, int _r) : l(_l), r(_r) {}
};

int N, m, sa[maxn], rank[maxn], rmq[maxn][25];
ll K;
char s[maxn];
SUB sub[maxn];

inline int LCP(int x, int y)
{
	x=rank[x]+1, y=rank[y]+1;
	if (x>y) swap(x,y);
	int k=31-__builtin_clz(y-x);
	return min(rmq[x][k],rmq[y-(1<<k)][k]);
}

inline bool operator<(const SUB &a, const SUB &b)
{
	if (a.r-a.l!=b.r-b.l) return a.r-a.l<b.r-b.l;
	if (a.l==b.l) return false;
	if (LCP(a.l,b.l)>=a.r-a.l+1) return false;
	return rank[a.l]<rank[b.l];
}

inline bool operator==(const SUB &a, const SUB &b)
{
	return !(a<b) && !(b<a);
}

void buildSA()
{
	static int tmp[maxn], rk[maxn<<1], tank[maxn];
	for (int i=1;i<=N;i++) tank[s[i]]++;
	for (int i=1;i<128;i++) tank[i]+=tank[i-1];
	for (int i=1;i<=N;i++) sa[tank[s[i]]--]=i;
	for (int i=1,j=0;i<=N;i++)
	{
		if (!j || s[sa[i]]>s[sa[i-1]]) j++;
		rank[sa[i]]=j;
	}
	for (int k=1;k<N;k<<=1)
	{
		int j(0);
		for (int i=N+1;i<=N+k;i++) tmp[++j]=i-k;
		for (int i=1;i<=N;i++) if (sa[i]>k) tmp[++j]=sa[i]-k;
		memset(tank,0,sizeof tank);
		for (int i=1;i<=N;i++) tank[rank[tmp[i]]]++;
		for (int i=1;i<=N;i++) tank[i]+=tank[i-1];
		for (int i=N;i;i--) sa[tank[rank[tmp[i]]]--]=tmp[i];
		memcpy(rk,rank,sizeof rank);
		for (int i=1,j=0;i<=N;i++)
		{
			if (!j || rk[sa[i]]>rk[sa[i-1]] || rk[sa[i]]==rk[sa[i-1]] && rk[sa[i]+k]>rk[sa[i-1]+k]) j++;
			rank[sa[i]]=j;
		}
	}
	for (int i=1;i<=N;i++)
	{
		rmq[rank[i]][0]=max(0,rmq[rank[i-1]][0]-1);
		while (s[i+rmq[rank[i]][0]]==s[sa[rank[i]-1]+rmq[rank[i]][0]]) rmq[rank[i]][0]++;
	}
	for (int k=1;(1<<k)<=N;k++)
		for (int i=1;i+(1<<k)-1<=N;i++)
			rmq[i][k]=min(rmq[i][k-1],rmq[i+(1<<k>>1)][k-1]);
}

void Manacher()
{
	static int f[maxn<<1];
	static char ss[maxn<<1];
	int n;
	ss[n=1]='#';
	for (int i=1;i<=N;i++)
		ss[++n]=s[i], ss[++n]='#';
	for (int i=1,t=0;i<=n;i++)
	{
		f[i]=max(0,min(t+f[t]-i,f[t+t-i]));
		if (t+f[t]<i && ss[i]!='#')
			sub[++m]=SUB(i>>1,i>>1);
		while (i+f[i]+1<=n && i-f[i]-1>0 && ss[i+f[i]+1]==ss[i-f[i]-1])
		{
			f[i]++;
			if (ss[i+f[i]]!='#')
				sub[++m]=SUB((i-f[i])>>1,(i+f[i])>>1);
		}
		if (i+f[i]>t+f[t]) t=i;
	}
}

int main()
{
	cin >> N >> K >> (s+1);
	buildSA();
	Manacher();
	sort(sub+1,sub+m+1);
	m=unique(sub+1,sub+m+1)-sub-1;
	K=K%m+1;
	s[sub[K].r+1]=0;
	cout << m << endl << (s+sub[K].l) << endl;
	return 0;
}
