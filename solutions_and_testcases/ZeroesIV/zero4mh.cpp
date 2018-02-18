#pragma warning(disable:4786)
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<set>
#include<map>
#include<functional>
#include<string>
#include<cstring>
#include<cstdlib>
#include<queue>
#include<utility>
#include<fstream>
#include<sstream>
#include<cmath>
#include<stack>
#include<assert.h>
using namespace std;

#define MEM(a, b) memset(a, (b), sizeof(a))
#define CLR(a) memset(a, 0, sizeof(a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(X) ( (X) > 0 ? (X) : ( -(X) ) )
#define S(X) ( (X) * (X) )
#define SZ(V) (int )V.size()
#define FORN(i, n) for(i = 0; i < n; i++)
#define FORAB(i, a, b) for(i = a; i <= b; i++)
#define ALL(V) V.begin(), V.end()

typedef pair<int,int> PII;
typedef pair<double, double> PDD;
typedef vector<int> VI;

#define IN(A, B, C) assert( B <= A && A <= C)

//typedef int LL;
typedef long long int LL;
//typedef __int64 LL;

LL dp[27][2][2][2];
int num[27];

LL DP(int dig, int flag, int parity, int last)
{
	if(dig == 0)
	{
		if(flag)
		{
			if(parity == 0) return num[dig] + 1;
			return 0;
		}
		else
		{
			if(parity == 0) return 5;
			return 0;
		}
	}

	LL &ret = dp[dig][flag][parity][last];
	int i;

	if(ret != -1) return ret;
	ret = 0;

	if(flag == 0)
	{
		for(i = 0; i < 5; i++)
			ret += DP(dig - 1, 0, (parity + last * 5 + i)&1, (last * 5 + i)&1);
	}

	if(flag == 1)
	{
		ret += DP(dig - 1, 1, (parity + last * 5 + num[dig])&1, (last * 5 + num[dig])&1);
		for(i = 0; i < num[dig]; i++)
			ret += DP(dig - 1, 0, (parity + last * 5 + i)&1, (last * 5 + i)&1);
	}

	return ret;
}

LL f(LL n)
{
	if(n <= 4) return n + 1;

	LL t = n;
	int dig = 0;
	while(t)
	{
		num[dig++] = t % 5;
		t /= 5;
	}

	assert(dig < 27);
	MEM(dp, -1);

	return DP(dig - 1, 1, 0, 0);
}

int main()
{
	LL n, LIM;
	int cnt;

	LIM = 10000000;
	LIM *= 100000000000ll;

	//while(scanf("%I64d", &n) != EOF && n != -1)
	while(scanf("%Ld", &n) != EOF && n != -1)
	{
		IN(n, 0, LIM);
		//printf("%I64d\n", f(n));
		printf("%Ld\n", f(n));
	}

	return 0;
}
