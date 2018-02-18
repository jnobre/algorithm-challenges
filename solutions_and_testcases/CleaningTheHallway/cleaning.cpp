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

#define MAXN 1003

//typedef int LL;
//typedef long long int LL;
//typedef __int64 LL;

#define eps 1e-6
#define zero(x) (((x)>0?(x):-(x))<eps)
#define _sign(x) ((x)>eps?1:((x)<-eps?-1:0))

double pi = acos(-1.);

struct point
{
	double x, y;
	point(){}
	point(double _x, double _y) {x = _x; y = _y;}
};

struct circle
{
	point p;
	double r;
	circle(){}
	circle(double _x, double _y, double _r) {r = _r; p = point(_x, _y);}
};

double dist(point p1,point p2){
	return sqrt(ABS((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)));
}

struct Ring
{
	circle c_out, c_in;
	void scan()
	{
		int x, y, D, d;

		scanf("%d %d %d %d", &x, &y, &D, &d);
		IN(x, -1000, 1000);
		IN(y, -1000, 1000);
		IN(D, 1, 200);
		IN(d, 1, 200);

		c_out = circle(x, y, D + d);
		c_in = circle(x, y, MAX(0, D - d));
	}

	int inside(point p)
	{
		return ( _sign(dist(p, c_out.p) - c_out.r) <= 0 && _sign(dist(p, c_in.p) - c_in.r) >= 0);
	}

};

Ring ring[MAXN];
int n;
vector<PDD> angles;

point intersection(point u1,point u2,point v1,point v2){
	point ret=u1;
	double t=((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
		/((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
	ret.x+=(u2.x-u1.x)*t;
	ret.y+=(u2.y-u1.y)*t;
	return ret;
}
int intersect_circle_circle(point c1,double r1,point c2,double r2){
	return dist(c1,c2)<r1+r2+eps&&dist(c1,c2)>fabs(r1-r2)-eps;
}
void intersection_line_circle(point c,double r,point l1,point l2,point& p1,point& p2){
	point p=c;
	double t;
	p.x+=l1.y-l2.y;
	p.y+=l2.x-l1.x;
	p=intersection(p,c,l1,l2);
	t=sqrt(ABS(r*r-dist(p,c)*dist(p,c)))/dist(l1,l2);
	p1.x=p.x+(l2.x-l1.x)*t;
	p1.y=p.y+(l2.y-l1.y)*t;
	p2.x=p.x-(l2.x-l1.x)*t;
	p2.y=p.y-(l2.y-l1.y)*t;
}

void intersection_circle_circle(point c1,double r1,point c2,double r2,point& p1,point& p2){
	point u,v;
	double t;
	t=(1+(r1*r1-r2*r2)/dist(c1,c2)/dist(c1,c2))/2;
	u.x=c1.x+(c2.x-c1.x)*t;
	u.y=c1.y+(c2.y-c1.y)*t;
	v.x=u.x+c1.y-c2.y;
	v.y=u.y-c1.x+c2.x;
	intersection_line_circle(c1,r1,u,v,p1,p2);
}

double Angle(point p, point q)
{
	q.x -= p.x;
	q.y -= p.y;

	if(q.x >= 0 && q.y >= 0) return atan2(q.y, q.x);
	else if(q.x <= 0 && q.y >= 0) return pi - atan2(q.y, -q.x);
	else if(q.x <= 0 && q.y <= 0) return pi + atan2(-q.y, -q.x);
	else return 2*pi - atan2(-q.y, q.x);
}

void ring_circle_intersect(circle &c, Ring &r)
{
	vector<double> angle;
	point p1, p2;
	double ang1, ang2, test;
	int i, sz;

	if(intersect_circle_circle(c.p, c.r, r.c_in.p, r.c_in.r))
	{
		intersection_circle_circle(c.p, c.r, r.c_in.p, r.c_in.r, p1, p2);
		ang1 = Angle(c.p, p1);
		ang2 = Angle(c.p, p2);

		angle.push_back(ang1);
		angle.push_back(ang2);
	}

	if(intersect_circle_circle(c.p, c.r, r.c_out.p, r.c_out.r))
	{
		intersection_circle_circle(c.p, c.r, r.c_out.p, r.c_out.r, p1, p2);
		ang1 = Angle(c.p, p1);
		ang2 = Angle(c.p, p2);

		angle.push_back(ang1);
		angle.push_back(ang2);
	}

	sort(ALL(angle));
	sz = SZ(angle);
	for(i = 0; i < sz; i++)
	{
		if(i < sz - 1)
			test = (angle[i] + angle[i + 1])/2;
		else
			test = (angle[i] + 2*pi + angle[0])/2;

		point p(c.p.x + c.r * cos(test), c.p.y + c.r * sin(test));
		if(r.inside(p))
		{
			if(i < sz - 1 && angle[i] <= pi && pi <= angle[i + 1])
			{
				angles.push_back(PDD(angle[i], pi));
				angles.push_back(PDD(pi, angle[i + 1]));
			}
			else if(i == sz - 1)
			{
				angles.push_back(PDD(angle[i], 2*pi));
				angles.push_back(PDD(0, angle[0]));
			}
			else
			{
				angles.push_back(PDD(angle[i], angle[i + 1]));
			}
		}
	}
}

double Signed_Segment_Area(circle &c, double from, double to)
{
	double dx1, dx2, dy1, dy2, t1, t2, area, xgap;

	dx1 = c.r * cos(from);
	dy1 = c.r * sin(from);

	dx2 = c.r * cos(to);
	dy2 = c.r * sin(to);

	t1 = from;
	t2 = to;

	area = 0.5*S(c.r) * (ABS(t2-t1) - sin(ABS(t2-t1)));//    -   0.5*xgap*(dy1 + dy2);
	xgap = ABS(dx1 - dx2);

	if(from < pi)
	{
		return area + 0.5*xgap*(dy1 + dy2 + 2*c.p.y);
	}
	else
		return -(-area + 0.5*xgap*(dy1 + dy2 + 2*c.p.y));
}

double Signed_Area(circle &c, int id)
{
	double ret = 0, d, now, prev;
	int i, sz;

	angles.clear();
	for(i = 0; i < n; i++) if(i != id)
	{
		d = dist(ring[i].c_out.p, c.p);
		if( _sign(d + c.r - ring[i].c_out.r) <= 0 && (_sign(d - ring[i].c_in.r - c.r) >= 0 || _sign(d + ring[i].c_in.r - c.r) <= 0))
			return 0;

		ring_circle_intersect(c, ring[i]);
	}

	angles.push_back(PDD(2*pi, 2*pi));
	sort(ALL(angles));

	sz = SZ(angles);
	now = 0;
	for(i = 0; i < sz; i++)
	{
		if(angles[i].first < now || zero(now - angles[i].first))
		{
			now = MAX(now, angles[i].second);
			continue;
		}

		prev = now;
		now = angles[i].first;

		if(prev <= pi && pi <= now)
		{
			ret += Signed_Segment_Area(c, prev, pi);
			ret += Signed_Segment_Area(c, pi, now);
		}
		else
		{
			ret += Signed_Segment_Area(c, prev, now);
		}

		now = angles[i].second;
	}

	return ret;
}

int main()
{
	//freopen("cleaning.in", "r", stdin);
	//freopen("cleaning.ans", "w", stdout);

	int T, ks;
	int i;
	double ans;

	scanf("%d", &T);
	IN(T, 1, 28);
	for(ks = 1; ks <= T; ks++)
	{
		fprintf(stderr, "%d\n", ks);

		scanf("%d", &n);
		for(i = 0; i < n; i++)
			ring[i].scan();

		IN(n, 1, 500);

		ans = 0;
		for(i = 0; i < n; i++)
		{
			ans += Signed_Area(ring[i].c_out, i);
			ans -= Signed_Area(ring[i].c_in, i);
		}

		printf("Case %d: %.6lf\n", ks, ans);
	}

	return 0;
}
