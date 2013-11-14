#include <point.h>
#include <cstdio>
#include <cmath>

template<typename T> static inline T sqr(const T &x)
{
	return (x) * (x);
}

const Point operator+(const Point& p, const Point& q)
{
	return Point(p.X + q.X, p.V + q.V, p.x + q.x, p.v + q.v, p.e + q.e);
}

const Point operator-(const Point& p, const Point& q)
{
	return Point(p.X - q.X, p.V - q.V, p.x - q.x, p.v - q.v, p.e - q.e);
}

const Point operator*(const ld& mult, const Point& p)
{
	return Point(p.X * mult, p.V * mult, p.x * mult, p.v * mult, p.e * mult);
}

const Point operator*(const Point& p, const ld& mult)
{
	return Point(p.X * mult, p.V * mult, p.x * mult, p.v * mult, p.e * mult);
}

const Point operator/(const Point& p, const ld &divisor)
{
	return Point(p.X / divisor, p.V / divisor, p.x / divisor, p.v / divisor, p.e / divisor);
}

const char *Point::to_string(char *buf)
{
	sprintf(buf, "%lf %lf %lf %lf %lf", X, V, x, v, e);
	return buf;
}

bool Point::operator==(const Point &other) const
{
	return fabs(X - other.X) < EPS &&
		   fabs(V - other.V) < EPS &&
		   fabs(x - other.x) < EPS &&
		   fabs(v - other.v) < EPS &&
		   fabs(e - other.e) < EPS;
}

bool Point::operator!=(const Point &other) const
{
	return !(*this == other);
}
