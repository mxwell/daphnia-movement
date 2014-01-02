#include <point.h>
#include <cstdio>
#include <cmath>

template<typename T> static inline T sqr(const T &x)
{
	return (x) * (x);
}

const Point operator+(const Point& p, const Point& q)
{
	return Point(p.X + q.X, p.Y + q.Y, p.Vx + q.Vx, p.Vy + q.Vy,
				 p.x + q.x, p.v + q.v, p.e + q.e, p.psi + q.psi);
}

const Point operator-(const Point& p, const Point& q)
{
	return Point(p.X - q.X, p.Y - q.Y, p.Vx - q.Vx, p.Vy - q.Vy,
				 p.x - q.x, p.v - q.v, p.e - q.e, p.psi - q.psi);
}

const Point operator*(const ld& mult, const Point& p)
{
	return Point(p.X * mult, p.Y * mult, p.Vx * mult, p.Vy * mult,
				 p.x * mult, p.v * mult, p.e * mult, p.psi * mult);
}

const Point operator*(const Point& p, const ld& mult)
{
	return mult * p;
}

const Point operator/(const Point& p, const ld &divisor)
{
	return Point(p.X / divisor, p.Y / divisor, p.Vx / divisor, p.Vy / divisor,
		 		 p.x / divisor, p.v / divisor, p.e / divisor, p.psi / divisor);
}

const char *Point::to_string()
{
	sprintf(buf, "%lf %lf %lf %lf %lf %lf %lf %lf",
					X, Y, Vx, Vy, x, v, e, psi);
	return buf;
}

const char *Point::pos_to_string()
{
	sprintf(buf, "%lf %lf", X, Y);
	return buf;
}

bool Point::operator==(const Point &other) const
{
	return fabs(X - other.X) 		< EPS &&
		   fabs(Y - other.Y) 		< EPS &&
		   fabs(Vx - other.Vx) 		< EPS &&
		   fabs(Vy - other.Vy)		< EPS &&
		   fabs(x - other.x) 		< EPS &&
		   fabs(v - other.v) 		< EPS &&
		   fabs(e - other.e) 		< EPS &&
		   fabs(psi - other.psi)	< EPS;
}

bool Point::operator!=(const Point &other) const
{
	return !(*this == other);
}

double Point::get_Vx() const
{
	return Vx;
}

double Point::get_Vy() const
{
	return Vy;
}
