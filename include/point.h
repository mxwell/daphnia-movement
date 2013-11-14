#ifndef __SSU_KMY_POINT_H_
#define __SSU_KMY_POINT_H_

#include <types.h>

/* Class describes state of modelled system and
 * gives a convenient way to write expressions
 * of integration
 */

class Point
{
	friend class Model;
public:
	Point() {}
	Point(ld X, ld V, ld x, ld v, ld e) :
		X(X), V(V), x(x), v(v), e(e) {}
	friend const Point operator+(const Point&, const Point&);
	friend const Point operator-(const Point&, const Point&);
	friend const Point operator*(const ld&, const Point&);
	friend const Point operator*(const Point&, const ld&);
	friend const Point operator/(const Point&, const ld&);
	const char *to_string(char *buf);
	bool operator==(const Point &other) const;
	bool operator!=(const Point &other) const;
private:
	/* position of particle */
	ld X;
	/* speed of particle */
	ld V;
	/* relative position of motor */
	ld x;
	/* relative speed of motor */
	ld v;
	/* depot */
	ld e;
};

#endif /* __SSU_KMY_POINT_H_ */
